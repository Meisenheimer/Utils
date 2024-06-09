#let Width = 16cm
#let Height = 12cm

#let FontSize = (
  "title": Height / 12,
  "subtitle": Height / 24,
  "frame-title": Height / 12,
  "text": Height / 18,
  "footer": Height / 48,
  "author": Height / 24,
  "institude": Height / 32,
  "date": Height / 24,
)

#let Color = (
  "title": rgb(255, 255, 255),
  "title-back": rgb(51, 51, 178),
  "left": rgb(25, 25, 89),
  "middle": rgb(38, 38, 134),
  "right": rgb(51, 51, 178),
)

#let FontBold = ("New Computer Modern", "SimHei")
#let FontNormal = ("New Computer Modern", "SimSun")
#let FontItalic = ("New Computer Modern", "KaiTi")
// "New Computer Modern", "Latin Modern Math", "Linux Libertine", "TeX Gyre Termes"

#let Date = state("Date", "")

#let slide(
  language: "en", // en, zh
  show_outline: true,
  title: "",
  subtitle: none,
  author: "",
  authors: (),
  date: datetime.today(),
  fontfamily: FontNormal,
  body,
) = {
  set document(author: authors.map(a => a.name), title: title)
  set page(
    width: Width,
    height: Height,
    margin: (x: 0pt, y: Height / 24),
    columns: 1,
    numbering: "1",
    number-align: center,
    header: context [],
    footer: context [
      #grid(
        columns: (Width / 3, Width / 3, Width / 3),
        rows: (7 * Height / 240, 7 * Height / 240, 7 * Height / 240),
        box(
          align(center + horizon)[#text(size: FontSize.at("footer"), author, fill: Color.at("title"))],
          height: 7 * Height / 240,
          width: Width / 3,
          fill: Color.at("left")),
        box(
          align(center + horizon)[#text(size: FontSize.at("footer"), title, fill: Color.at("title"))],
          height: 7 * Height / 240,
          width: Width / 3,
          fill: Color.at("middle")),
        box(
          align(center + horizon)[#text(size: FontSize.at("footer"), Date.get(), fill: Color.at("title"))],
          height: 7 * Height / 240,
          width: Width / 3,
          fill: Color.at("right")),
      )
      // #move(dx: 0cm, dy: -Height / 15)[
      #move(dx: 0cm, dy: -Height * 23 / 240)[
        #box(
          align(right + horizon)[#text(size: FontSize.at("footer"), counter(page).display(), fill: Color.at("title"))],
          height: 7 * Height / 240,
          width: 63 * Width / 64,
        )
      ]
    ]
  )
  set text(font: fontfamily, lang: language, size: FontSize.at("text"))

  if language == "en" {
    Date.update(date.display("[month repr:long] [day], [year]"))
  }
  if language == "zh" {
    Date.update(date.display("[year]年[month]月[day]日"))
  }

  // Title Page
  align(center)[
    #v(Height / 6, weak: false)
    #if subtitle == none [
      #block(
        align(center + horizon)[#text(FontSize.at("title"), title, fill: Color.at("title"))],
        width: 0.9 * Width,
        height: Height / 8,
        fill: Color.at("title-back"),
        inset: Height / 32,
        radius: Height / 64,
      )
    ]
    #if subtitle != none [
      #block(
        align(center + horizon)[
          #text(FontSize.at("title"), title, fill: Color.at("title"))
          #v(Height / 24, weak: true)
          #text(FontSize.at("subtitle"), subtitle, fill: Color.at("title"))
        ],
        width: 0.9 * Width,
        height: Height / 5,
        fill: Color.at("title-back"),
        inset: Height / 32,
        radius: Height / 64,
      )
    ]
    #v(2em, weak: true)
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
      #v(1em, weak: true)
      #if author.institude != none [
        #set text(size: FontSize.at("institude"))
        #author.institude
      ]
    ]),
  ))

  align(center)[
    #v(Height / 12, weak: true)
    #if language == "en" [
      #text(FontSize.at("date"), date.display("[month repr:long] [day], [year]"))
    ]
    #if language == "zh" [
      #text(FontSize.at("date"), date.display("[year]年[month]月[day]日"))
    ]
  ]

  // Main body.
  show strong: set text(font: FontBold)
  show emph: set text(font: FontItalic)
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
      counter("env").update(counter(heading).get())
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
  // Body
  counter(page).update(1)
  set par(justify: true, first-line-indent: 2em)
  body


  // Thanks page
  set page(header: context [], footer: context [])
  pagebreak(weak: false)
  set text(size: FontSize.at("title"))
  align(center + horizon)[
    #if language == "en" [
      Thank you for listening
    ]
    #if language == "zh" [
      感谢各位聆听 \
      请批评指正
    ]
  ]
}

#let frame(title, body) = {
  pagebreak(weak: false)
  move(dx: 0cm, dy: -Height / 24)[
    #box(
      align(left + horizon)[#text(size: FontSize.at("title"), "  " + title, fill: Color.at("title"))],
      height: Height / 8,
      width: Width,
      fill: Color.at("title-back")
    )
  ]
  set par(
    justify: true,
    linebreaks: "optimized",
    first-line-indent: 0em,
  )
  set text(size: FontSize.at("text"))
  align(center + horizon)[
    #move(dx: 0cm, dy: -Height / 16)[
      #box(
          align(left + horizon)[#body],
          height: 0.7 * Height,
          width: 0.9 * Width,
          // fill: rgb(200, 200, 200),
      )
    ]
  ]
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
