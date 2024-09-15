#import "slide.typ": *

#show: slide.with(
  title: "A Title is all you need",
  language: "zh",
  show_outline: true,
  subtitle: "A subtitle is all you need",
  author: "name(institude)",
  authors: (
    (
      name: "name",
      institude: "institude",
      corresponding: true,
    ),
  ),
)

#frame("title")[
  body body body body body body body body body body body body body body body body body body body body

  body body body body body body body body body body body body body body body body body body body body

]

#frame("")[
  #align(center + horizon)[
    #set text(size: FontSize.at("title"))
    感谢各位聆听 \
    请批评指正
  ]
]
