
#let FontSize = (
  // EN: Huge-size: 24.88pt, huge-size: 20.74pt, LARGE-size: 17.28pt, Large-size: 14.4pt, large-size: 12pt, normal-size: 10.95pt, small-size: 10pt, footnote-size: 9pt, script-size: 8pt, tiny-size: 6pt
  // CN: 初号: 42pt, 小初: 36pt, 一号: 26pt, 小一: 24pt, 二号: 22pt, 小二: 18pt, 三号: 16pt, 小三: 15pt, 四号: 14pt, 小四: 12pt, 五号: 10.5pt, 小五: 9pt, 六号: 7.5pt, 小六: 6.5pt, 七号: 5.5pt, 八号: 5pt,
  // article 17.28pt, 14.4pt, 12pt, 10.95pt, 10pt, 9pt, 8pt, 6pt.
  // article section, subsection, subsubsection, text, others
  // book 24.88pt, 20.74pt, 17.28pt, 14.4pt, 12pt, 10.95pt, 10pt, 9pt, 8pt, 6pt.
  // book part, chapter, section, subsection, subsubsection, text, others
  "part": 24.88pt,
  "level-1": 20.74pt,
  "level-2": 17.28pt,
  "level-3": 14.4pt,
  "level-4": 12pt,
  "level-5": 10.95pt,
  "text": 10.95pt,
  "header": 14.4pt,
  "author": 14.4pt,
  "institude": 12pt,
  "date": 14.4pt,
  "abstract": 14.4pt,
  "outline": 12pt,
)

#let FontBold = ("New Computer Modern", "SimHei")
#let FontNormal = ("New Computer Modern", "SimSun")
#let FontItalic = ("New Computer Modern", "KaiTi")
// "New Computer Modern", "Latin Modern Math", "Linux Libertine", "TeX Gyre Termes"

#let book(
  papersize: "a4", // 21(cm) * 29.7(cm)
  language: "en", // en, zh
  show_outline: true,
  title: "",
  subtitle: none,
  authors: (),
  date: datetime.today(),
  preface: none,
  fontfamily: FontNormal,
  bibliography_path: none,
  body,
) = {
  set document(author: authors.map(a => a.name), title: title)
  set page(
    paper: papersize,
    columns: 1,
    numbering: "1",
    number-align: center,
    header: locate(loc => {
      if counter(page).at(loc).first() > 1 [
        #set text(size: FontSize.at("header"))
        #set align(center)
        #title
        #line(length: 100%)
      ]}),
  )
  set text(font: fontfamily, lang: language, size: FontSize.at("text"))
  show par: set block(above: 0.58em, below: 0.58em)

  show heading: it => {
    set text(size: FontSize.at("text"), weight: "bold")
    set par(first-line-indent: 0pt)
    set align(center)
    set text(size: FontSize.at("level-1"))
    v(FontSize.at("level-1"), weak: true)
    strong(it.body)
    v(FontSize.at("level-2"), weak: true)
  }

  // Title Page
  set page(numbering: "A")
  {
    show heading: none
    if language == "en" {
      hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "Cover"))
    }
    if language == "zh" {
      hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "封面"))
    }
  }
  align(center)[
    #v(20em, weak: false)
    #text(2em, title) \
    #if subtitle != none [
      #text(1.2em, subtitle) \
    ]
    #v(2.5em, weak: true)
  ]

  let parse_authors(authors) = {
    let affiliations = ()
    let parsed_authors = ()
    let corresponding = ()
    let pos = 0
    for author in authors {
      if "affiliation" in author {
        if author.affiliation not in affiliations {
          affiliations.push(author.affiliation)
        }
        pos = affiliations.position(a => a == author.affiliation)
        author.insert("affiliation_parsed", pos)
      } else {
        // if author has no affiliation, just use the same as the previous author
        author.insert("affiliation_parsed", pos)
      }
      parsed_authors.push(author)
      if "corresponding" in author {
        if author.corresponding {
          corresponding = author
        }
      }
    }
    (
      authors: parsed_authors,
      affiliations: affiliations,
      corresponding: corresponding,
    )
  }

  let authors_parsed = parse_authors(authors)

  // List Authors
  pad(top: 0.3em, bottom: 0.3em, x: 2em, grid(
    columns: (1fr,) * calc.min(3, authors_parsed.authors.len()),
    gutter: 1em,
    ..authors_parsed.authors.map(author => align(center)[
      #set text(size: FontSize.at("author"))
      #author.name
      #if author.email != none [
        #footnote[Email: #link("mailto:" + authors_parsed.corresponding.email, authors_parsed.corresponding.email)] \
      ]
      #if author.institude != none [
        #set text(size: FontSize.at("institude"))
        #author.institude
      ]
    ]),
  ))

  align(center)[
    #v(2.5em, weak: true)
    #if language == "en" [
      #text(FontSize.at("date"), date.display("[month repr:long] [day], [year]"))
    ]
    #if language == "zh" [
      #text(FontSize.at("date"), date.display("[year]年[month]月[day]日"))
    ]
    #v(5em, weak: true)
  ]
  // set par(justify: true)

  // Preface
  if preface != none [
    #pagebreak(weak: true)
    #if language == "en" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        [Preface])
    ]
    #if language == "zh" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        [前言])
    ]
    #preface
    #pagebreak(weak: true)
  ]

  // Main body.
  show strong: set text(font: FontBold)
  show emph: set text(font: FontItalic)
  set page(numbering: "i")
  set enum(numbering: "(1)", indent: 2em)
  set list(indent: 2em)

  counter("env").update((0, 0))
  counter("part").update(0)
  set heading(numbering: "1.1 ")
  show heading: it => {
    let number = if it.numbering != none {
      counter(heading).display(it.numbering)
      h(7pt, weak: true)
    }
    set par(first-line-indent: 0pt)
    if it.level == 1 {
      pagebreak(weak: true)
      counter("env").update(counter(heading).get())
      set text(size: FontSize.at("level-1"))
      v(FontSize.at("level-1"), weak: true)
      if language == "en" {
        "Chapter " + number + "\n"
        v(FontSize.at("level-2"), weak: true)
        strong(it.body)
      }
      if language == "zh" {
        strong("第  ") + number + strong("章  ") + strong(it.body)
      }
      v(FontSize.at("level-2"), weak: true)
    } else if it.level == 2 {
      set text(size: FontSize.at("level-2"))
      v(FontSize.at("level-2"), weak: true)
      number
      strong(it.body)
      v(FontSize.at("level-3"), weak: true)
    } else if it.level == 3 {
      set text(size: FontSize.at("level-3"))
      v(FontSize.at("level-3"), weak: true)
      number
      strong(it.body)
      v(FontSize.at("level-4"), weak: true)
    } else if it.level == 4 {
      set text(size: FontSize.at("level-4"))
      v(FontSize.at("level-4"), weak: true)
      number
      strong(it.body)
      v(FontSize.at("level-5"), weak: true)
    } else {
      set text(size: FontSize.at("level-5"))
      v(FontSize.at("level-5"), weak: true)
      strong(it.body)
      v(FontSize.at("level-5"), weak: true)
    }
  }

  // Outline
  if show_outline == true [
    #show heading: it => {
      set par(first-line-indent: 0pt)
      set text(size: FontSize.at("level-1"))
      v(FontSize.at("level-1"), weak: true)
      strong(it.body)
      v(FontSize.at("level-2"), weak: true)
    }
    #show outline.entry.where(level: 1): it => {
      v(FontSize.at("text"), weak: true)
      strong(it)
    }
    #counter(page).update(1)
    #pagebreak(weak: true)
    #set text(size: FontSize.at("outline"))
    #if language == "zh" [
      #{
        show heading: none
        hide(
          heading(
            level: 1,
            numbering: none,
            outlined: false,
            bookmarked: true,
            "目录")
        )
      }
      #outline(title: [目录], depth: 3, indent: 1.25em)
    ]
    #if language == "en" [
      #{
        show heading: none
        hide(
          heading(
            level: 1,
            numbering: none,
            outlined: false,
            bookmarked: true,
            "Contents")
        )
      }
      #outline(title: [Contents], depth: 3, indent: 1.25em)
    ]
    #pagebreak(weak: true)
  ]

  // Body
  set page(numbering: "1")
  counter(page).update(1)
  set par(justify: true, first-line-indent: 2em)
  body

  // Bibliography
  if bibliography_path != none {
    show heading: it => {
      set par(first-line-indent: 0pt)
      set text(size: FontSize.at("level-1"))
      v(FontSize.at("level-1"), weak: true)
      strong(it.body)
      v(FontSize.at("level-2"), weak: true)
    }

    pagebreak(weak: true)
    if language == "zh" {
      bibliography(bibliography_path, title: "参考文献", style: "gb-7714-2015-numeric")
    }
    if language == "en" {
      bibliography(bibliography_path, title: "Bibliography", style: "association-for-computing-machinery")
    }
    show bibliography: set block(spacing: 0.58em)
    show bibliography: set par(first-line-indent: 0em)
  }
}

#let head(title, level: 1) = {
  show heading: it => {
    set par(first-line-indent: 0pt)
    if it.level == 1 {
      counter("env").update(0)
      set text(size: FontSize.at("level-1"))
      v(FontSize.at("level-1"), weak: true)
      strong(it.body)
      v(FontSize.at("level-2"), weak: true)
    } else if it.level == 2 {
      set text(size: FontSize.at("level-2"))
      v(FontSize.at("level-2"), weak: true)
      strong(it.body)
      v(FontSize.at("level-3"), weak: true)
    } else if it.level == 3 {
      set text(size: FontSize.at("level-3"))
      v(FontSize.at("level-3"), weak: true)
      strong(it.body)
      v(FontSize.at("level-4"), weak: true)
    } else if it.level == 4 {
      set text(size: FontSize.at("level-4"))
      v(FontSize.at("level-4"), weak: true)
      strong(it.body)
      v(FontSize.at("level-5"), weak: true)
    } else {
      set text(size: FontSize.at("level-5"))
      v(FontSize.at("level-5"), weak: true)
      strong(it.body)
      v(FontSize.at("level-5"), weak: true)
    }
  }
  heading(level: level, numbering: none, supplement: none, outlined: true, bookmarked: true, title)
}

#let part(title) = {
  pagebreak(weak: true)
  counter("part").step()
  align(center)[
    #hide(heading(level: 1, numbering: none, outlined: true, bookmarked: true, title))
    #v(20em, weak: true)
    #set text(size: FontSize.at("part"), weight: "bold")
    Part #counter("part").display("1") \
    #v(1em, weak: true)
    #title
  ]
  pagebreak(weak: true)
}

#let env(title, body, name: none, number: true) = {
  v(1em, weak: true)
  set text(size: FontSize.at("text"), weight: "bold")
  set par(first-line-indent: 0em, justify: true)
  title + " "
  if number == true {
    counter("env").step(level: 2)
    counter("env").display("1.")
  }
  if name != none {
    " (" + name + ")"
  }
  set text(size: FontSize.at("text"), weight: "regular")
  " " + body
  v(2em, weak: true)
}

#let mathbf(body) = {
  $upright(bold(body))$
}
