#import "@local/font:1.0.0": *

#let env(title, body, name: none, number: true, tag: none) = {
  v(2em, weak: true)
  context {
    block(width: 100%, breakable: true)[
      #set par(first-line-indent: 0em, justify: true)
      #box[
        #set text(size: FontSize.at("text"), weight: "bold", style: "normal")
        #title
        #if number == true {
          counter("env").step(level: 2)
          counter("env").display("1.")
        }
        #if tag != none {label(tag)}
      ]
      #set text(size: FontSize.at("text"), weight: "bold", style: "normal")
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

#let mathcal(body) = math.class("normal", box({
  show math.equation: set text(font: "Garamond-Math", stylistic-set: 3)
  $#math.cal(body)$
}))

#let envref(label) = {
  show ref: it => {
    link(label)[#it.element]
  }

  ref(label)
}