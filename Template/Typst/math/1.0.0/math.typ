#import "@local/font:1.0.0": *

#let env(title, body, name: none, number: true) = {
  v(2em, weak: true)
  context {
    block(width: 100%, breakable: false)[
      #set text(size: FontSize.at("text"), weight: "bold", style: "normal")
      #set par(first-line-indent: 0em, justify: true)
      #title
      #if number == true {
        counter("env").step(level: 2)
        counter("env").display("1.")
      }
      #if name != none {
        "(" + name + ")"
      }
      #set text(size: FontSize.at("text"), weight: "regular", style: "normal")
      #body
    ]
  }
  v(1em, weak: true)
}

#let proof(body) = {
  v(1em, weak: true)
  set text(size: FontSize.at("text"), weight: "bold")
  set par(first-line-indent: 0em, justify: true)
  set text(size: FontSize.at("text"), weight: "regular", style: "italic")
  "Proof. "
  set text(size: FontSize.at("text"), weight: "regular", style: "normal")
  body + h(1fr) + $square.filled$
  v(2em, weak: true)
}

#let solution(body) = {
//   v(1em, weak: true)
  set text(size: FontSize.at("text"), weight: "bold")
  set par(first-line-indent: 0em, justify: true)
  set text(size: FontSize.at("text"), weight: "regular", style: "italic")
  table(columns: (1fr), inset: 0.5em, stroke: 1pt)[
    #{
        "Solution. "
        set text(size: FontSize.at("text"), weight: "regular", style: "normal")
        body
    }
  ]
//   v(2em, weak: true)
}

#let mathbf(body) = {
  $upright(bold(body))$
}
