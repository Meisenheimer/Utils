#let tiny-size =  6pt
#let script-size =  8pt
#let footnote-size =  9pt
#let small-size = 10pt
#let normal-size = 10.95pt
#let large-size = 12pt
#let Large-size = 14.4pt
#let LARGE-size = 17.28pt
#let huge-size =  20.74pt
#let Huge-size =  24.88pt

#let FontBold = ("New Computer Modern", "SimHei")
#let FontNormal = ("New Computer Modern", "SimSun")
#let FontItalic = ("New Computer Modern", "KaiTi")
// "New Computer Modern", "Latin Modern Math", "Linux Libertine", "TeX Gyre Termes"

#let project(
  doctype: "article", // article, book.
  papersize: "a4", // 21(cm) * 29.7(cm)
  language: "en", // en, cn
  show_outline: true,
  title: "",
  subtitle: none,
  authors: (),
  date: datetime.today(),
  abstract: none,
  keywords: none,
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
        #set text(size: large-size)
        #if doctype == "article" [
          #title
          #h(1fr)
          #if language == "en" [
            #date.display("[month repr:long] [day], [year]")
          ]
          #if language == "cn" [
            #date.display("[year]年[month]月[day]日")
          ]
        ]
        #if doctype == "book" [
          #set align(center)
          #title
        ]
        #line(length: 100%)
      ]}),
  )
  set text(font: fontfamily, lang: language, size: normal-size)
  show par: set block(above: 0.58em, below: 0.58em)

  show heading: it => {
    set text(size: normal-size, weight: "bold")
    set par(first-line-indent: 0pt)
    if doctype == "article" {
      set align(center)
      set text(size: LARGE-size)
      v(LARGE-size, weak: true)
      it.body
      v(normal-size, weak: true)
    }
    if doctype == "book" {
      set align(center)
      set text(size: huge-size)
      v(Huge-size, weak: true)
      it.body
      v(Huge-size, weak: true)
    }
  }

  // Title Page
  set page(numbering: "A")
  align(center)[
    #if doctype == "article" [
      #v(10em, weak: false)
    ]
    #if doctype == "book" [
      #v(20em, weak: false)
    ]
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
      #set text(size: Large-size)
      #author.name
      #if author.email != none [
        #footnote[Email: #link("mailto:" + authors_parsed.corresponding.email, authors_parsed.corresponding.email)] \
      ]
      #if author.institude != none [
        #set text(size: large-size)
        #author.institude
      ]
    ]),
  ))

  align(center)[
    #v(2.5em, weak: true)
    #if language == "en" [
      #text(Large-size, date.display("[month repr:long] [day], [year]"))
    ]
    #if language == "cn" [
      #text(Large-size, date.display("[year]年[month]月[day]日"))
    ]
    #v(5em, weak: true)
  ]
  // set par(justify: true)

  // Abstract & Keywords
  if abstract != none [
    #v(5em, weak: true)
    #if doctype == "book" [
      #pagebreak(weak: true)
    ]
    #if language == "en" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        text(Large-size, weight: "bold", [Abstract]))
    ]
    #if language == "cn" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        text(Large-size, weight: "bold", [摘要]))
    ]
    #align(center)[
      #block(width: 90%)[
        #align(left)[
          #abstract \
          #v(1em, weak: true)
          #if keywords != none [
            #if language == "en" [
              *Keywords:*  #keywords
            ]
            #if language == "cn" [
              *关键词：*  #keywords
            ]
          ]
        ]
      ]
    ]
    #pagebreak(weak: true)
  ]

  // Preface
  if preface != none and doctype == "book" [
    #pagebreak(weak: true)
    #if language == "en" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        [Preface])
    ]
    #if language == "cn" [
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
    set text(size: normal-size, weight: "bold")
    set par(first-line-indent: 0pt)
    if it.level == 1 {
      counter("env").update(counter(heading).get())
      if doctype == "article" {
        set text(size: LARGE-size)
        v(LARGE-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        if language == "cn" {
          pagebreak(weak: true)
          set text(size: huge-size)
          v(Huge-size, weak: true)
          set align(center)
          "第  " + number + "章\n"
          it.body
          v(Huge-size, weak: true)
        }
        if language == "en" {
          pagebreak(weak: true)
          set text(size: huge-size)
          v(Huge-size, weak: true)
          "Chapter "
          number
          "\n"
          v(Huge-size, weak: true)
          it.body
          v(Huge-size, weak: true)
        }
      }
    } else if it.level == 2 {
      if doctype == "article" {
        set text(size: Large-size)
        v(Large-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        set text(size: LARGE-size)
        v(LARGE-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
    } else if it.level == 3 {
      if doctype == "article" {
        set text(size: large-size)
        v(large-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        set text(size: Large-size)
        v(Large-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
    } else if it.level == 4 {
      if doctype == "article" {
        set text(size: nromal-size)
        v(normal-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        set text(size: large-size)
        v(large-size, weak: true)
        number
        it.body
        v(normal-size, weak: true)
      }
    } else {
      set text(size: normal-size)
      v(normal-size, weak: true)
      it.body
      v(normal-size, weak: true)
    }
  }

  // Outline
  if show_outline == true [
    #show heading: it => {
      set par(first-line-indent: 0pt)
      if doctype == "article" {
        set text(size: LARGE-size)
        v(LARGE-size, weak: true)
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        set text(size: huge-size)
        v(Huge-size, weak: true)
        it.body
        v(Huge-size, weak: true)
      }
    }
    #counter(page).update(1)
    #if doctype == "book" [
      #pagebreak(weak: true)
    ]
    #set text(size: large-size)
    #if language == "cn" [
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
      if doctype == "article" {
        set text(size: LARGE-size)
        v(LARGE-size, weak: true)
        it.body
        v(normal-size, weak: true)
      }
      if doctype == "book" {
        set text(size: huge-size)
        v(Huge-size, weak: true)
        it.body
        v(Huge-size, weak: true)
      }
    }

    pagebreak(weak: true)
    if language == "cn" {
      bibliography(bibliography_path, title: "参考文献", style: "gb-7714-2015-numeric")
    }
    if language == "en" {
      bibliography(bibliography_path, title: "Bibliography", style: "association-for-computing-machinery")
    }
    show bibliography: set block(spacing: 0.58em)
    show bibliography: set par(first-line-indent: 0em)
  }
}

#let part(title) = {
  pagebreak(weak: true)
  counter("part").step()
  align(center)[
    #hide(
      heading(
        level: 1,
        numbering: none,
        outlined: true,
        bookmarked: true,
        title)
    )
    #v(20em, weak: true)
    #set text(size: Huge-size, weight: "bold")
    Part #counter("part").display("1") \
    #v(1em, weak: true)
    #title
  ]
  pagebreak(weak: true)
}

#let env(title, body, name: none) = {
  v(1em, weak: true)
  set text(size: normal-size, weight: "bold")
  set par(first-line-indent: 0em, justify: true)
  title
  " "
  counter("env").step(level: 2)
  counter("env").display("1.")
  if name != none {
    " ("
    name
    ")"
  }
  " "
  set text(size: normal-size, weight: "regular")
  body
  v(2em, weak: true)
}

#let mathbf(body) = {
  $upright(bold(body))$
}