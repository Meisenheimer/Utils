#import "@local/font:1.0.0": *

#let indent_size = ("zh": 2em, "en": 0em)

#let doc(
  documentclass: "article", // article, book, homework
  papersize: "a4", // 21(cm) * 29.7(cm)
  language: "en", // en, zh
  show_outline: none,
  title: "",
  subtitle: none,
  authors: (),
  date: datetime.today(),
  abstract: none,
  keywords: none,
  preface: none,
  fontfamily: FontNormal,
  reference: none,
  body,
) = {
  set document(author: authors.map(a => a.name), title: title)
  set page(
    paper: papersize,
    columns: 1,
    numbering: "1",
    number-align: center,
    header: [
      #set par(spacing: 0.5em)
      #context [
        #if counter(page).get().first() >= 2 [
          #if documentclass == "article" [
            #set text(size: FontSize.at("header"))
            #title
            #h(1fr)
            #if language == "en" [
                #date.display("[month repr:long] [day], [year]")
            ]
            #if language == "zh" [
                #date.display("[year]年[month]月[day]日")
            ]
            #line(length: 100%)
          ]
          #if documentclass == "book" [
            #set text(size: FontSize.at("header"))
            #set align(center)
            #title
            #line(length: 100%)
          ]
          #if documentclass == "homework" [
            #set text(size: FontSize.at("header"))
            #title
            #h(1fr)
            #if language == "en" [
                #date.display("[month repr:long] [day], [year]")
            ]
            #if language == "zh" [
                #date.display("[year]年[month]月[day]日")
            ]
            #line(length: 100%)
          ]
        ]
      ]
    ],
  )
  set text(font: fontfamily, lang: language, size: FontSize.at("text"))

  counter(page).update(1)
  set par(justify: true, first-line-indent: indent_size.at(language))

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
  align(center)[
    #if documentclass == "article" [
      #v(6em, weak: false)
      #text(2em, title) \
      #if subtitle != none [
      #text(1.2em, subtitle) \
      ]
      #v(2.5em, weak: true)
    ]
    #if documentclass == "book" [
      #set page(numbering: "A")
      #show heading: none
      #if language == "en" {
          hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "Cover"))
      }
      #if language == "zh" {
          hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "封面"))
      }
      #v(20em, weak: false)
      #text(2em, title) \
      #if subtitle != none [
      #text(1.2em, subtitle) \
      ]
      #v(2.5em, weak: true)
    ]
    #if documentclass == "homework" [
      #v(7.5em, weak: false)
      #text(2em, title) \
      #if subtitle != none [
      #text(1.2em, subtitle) \
      ]
      #v(2.5em, weak: true)
    ]
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

  set par(justify: true, first-line-indent: indent_size.at(language))

  // Abstract & Keywords
  if abstract != none and documentclass == "article" [
    #v(5em, weak: true)
    #if language == "en" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        text(FontSize.at("abstract"), weight: "bold", [Abstract]))
    ]
    #if language == "zh" [
      #heading(
        outlined: false,
        bookmarked: true,
        numbering: none,
        text(FontSize.at("abstract"), weight: "bold", [摘要]))
    ]
    #align(center)[
      #block(width: 90%)[
        #align(left)[
          #abstract \
          #v(1em, weak: true)
          #if keywords != none [
            #if language == "en" [
              *Keywords:*    #keywords
            ]
            #if language == "zh" [
              *关键词：*    #keywords
            ]
          ]
        ]
      ]
    ]
    #pagebreak(weak: true)
  ]

  // Preface
  if preface != none and documentclass == "book" [
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
  set enum(numbering: "(1)", indent: 2em)
  set list(indent: 2em)

  counter("env").update((0, 1))
  counter("part").update(0)
  set heading(numbering: "1.1 ")
  show heading: it => {
    if documentclass == "article" {
      let number = if it.numbering != none {
        counter(heading).display(it.numbering)
        h(7pt, weak: true)
      }
      set par(first-line-indent: 0pt)
      if it.level == 1 {
        counter("env").update(counter(heading).get())
        counter("env").step(level: 2)
        set text(size: FontSize.at("level-1"))
        v(FontSize.at("level-1"), weak: true)
        number
        strong(it.body)
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
    if documentclass == "book" {
      let number = if it.numbering != none {
        counter(heading).display(it.numbering)
        h(7pt, weak: true)
      }
      set par(first-line-indent: 0pt)
      if it.level == 1 {
        pagebreak(weak: true)
        counter("env").update(counter(heading).get())
        counter("env").step(level: 2)
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
    if documentclass == "homework" {
      let number = if it.numbering != none {
        counter(heading).display(it.numbering)
        h(7pt, weak: true)
      }
      set par(first-line-indent: 0pt)
      if it.level == 1 {
        counter("env").update(counter(heading).get())
        counter("env").step(level: 2)
        set text(size: FontSize.at("level-1"))
        v(FontSize.at("level-1"), weak: true)
        number
        set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        it.body
        v(FontSize.at("level-2"), weak: true)
      } else if it.level == 2 {
        set text(size: FontSize.at("level-2"))
        v(FontSize.at("level-2"), weak: true)
        number
        set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        strong(it.body)
        v(FontSize.at("level-3"), weak: true)
      } else if it.level == 3 {
        set text(size: FontSize.at("level-3"))
        v(FontSize.at("level-3"), weak: true)
        number
        set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        strong(it.body)
        v(FontSize.at("level-4"), weak: true)
      } else if it.level == 4 {
        set text(size: FontSize.at("level-4"))
        v(FontSize.at("level-4"), weak: true)
        number
        set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        strong(it.body)
        v(FontSize.at("level-5"), weak: true)
      } else {
        set text(size: FontSize.at("level-5"))
        v(FontSize.at("level-5"), weak: true)
        set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        strong(it.body)
        v(FontSize.at("level-5"), weak: true)
      }
    }
  }

  // Outline
  if show_outline == true [
    #set page(numbering: "i")
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
        hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "目录"))
      }
      #outline(title: [目录], depth: 3, indent: 1.25em)
    ]
    #if language == "en" [
      #{
        show heading: none
        hide(heading(level: 1, numbering: none, outlined: false, bookmarked: true, "Contents"))
      }
      #outline(title: [Contents], depth: 3, indent: 1.25em)
    ]
    #pagebreak(weak: true)
  ]

  // Body
  show math.equation: set block(breakable: true)
  if documentclass != "homework" {
    counter(page).update(1)
    set par(justify: true, first-line-indent: indent_size.at(language))
    body
  } else {
    set page(numbering: "1")
    counter(page).update(1)
    set par(justify: true, first-line-indent: indent_size.at(language))
    body
  }

  // Bibliography
  if reference != none {
    set page(numbering: "1")
    show heading: it => {
      set par(first-line-indent: 0pt)
      set text(size: FontSize.at("level-1"))
      v(FontSize.at("level-1"), weak: true)
      strong(it.body)
      v(FontSize.at("level-2"), weak: true)
    }
    pagebreak(weak: true)
    reference
    show bibliography: set block(spacing: 0.58em)
    show bibliography: set par(first-line-indent: 0em)
  }
}

#let head(title, level: 1) = {
  show heading: it => {
    set par(first-line-indent: 0pt)
    if it.level == 1 {
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
  context {
    counter("part").step()
    align(center)[
      #hide(heading(level: 1, numbering: none, outlined: true, bookmarked: true, title))
      #v(20em, weak: true)
      #set text(size: FontSize.at("part"), weight: "bold")
      Part #counter("part").display("1") \
      #v(1em, weak: true)
      #title
    ]
  }
  pagebreak(weak: true)
}

#let reference(ref) = {
  set page(numbering: "1")
  show heading: it => {
    set par(first-line-indent: 0pt)
    set text(size: FontSize.at("level-1"))
    v(FontSize.at("level-1"), weak: true)
    strong(it.body)
    v(FontSize.at("level-2"), weak: true)
  }
  pagebreak(weak: true)
  ref
  show bibliography: set block(spacing: 0.58em)
  show bibliography: set par(first-line-indent: 0em)
}

#let appendix(body) = {
  pagebreak(weak: true)
  counter(heading).update(0)
  set heading(numbering: "A.1")
  body
}