#import "doc.typ": *
#import "@local/math:1.0.0": *

#show: doc.with(
  // documentclass: "article",
  documentclass: "book",
  // documentclass: "homework",
  title: "A Title is all you need",
  language: "en",
  show_outline: true,
  subtitle: none,
  authors: (
    (
      name: "name",
      email: "email",
      institude: "institude",
      corresponding: true,
    ),
  ),
  abstract: [Title of a scientific paper is an important element that conveys the main message of the study to the readers. In this study, we investigate the impact of paper titles on citation count, and propose that the title alone has the highest impact on citation count. Using a dataset of 1000 scientific papers from various disciplines, we analyzed the correlation between the characteristics of paper titles, such as length, clarity, novelty, and citation count. Our results show that papers with clear, concise, and novel titles tend to receive more citations than those with longer or less informative titles. Moreover, we found that papers with creative and attention-grabbing titles tend to attract more readers and citations, which supports our hypothesis that the title alone has the highest impact on citation count. Our findings suggest that researchers should pay more attention to crafting effective titles that accurately and creatively summarize the main message of their research, as it can have a significant impact on the success and visibility of their work.],
  keywords: [content analysis, citation, bibliometrics],
  preface: [this is a preface.],
  bibliography_path: "reference.bib",
)

#part("1")
#part("2")

#strong("中文") #emph("中文") *中文* _中文_ #text("中文", font: FontBold) #text("中文", font: FontItalic)

= Introduction

= 中文标题
== 中文标题
=== 中文标题
==== 中文标题

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

#head("title", level: 1)

#head("subtitle", level: 2)

#head("subsubtitle", level: 3)

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

中文测试

Title of a scientific paper is an important element @teixeira2015importance that
conveys the main message of the study to the readers @hartley2019academic. In
this study, we investigate the impact of paper titles on citation count, and
propose that the title alone has the highest impact on citation count.

Using a dataset of 1000 scientific papers from various disciplines, we analyzed
the correlation between the characteristics of paper titles, such as length,
clarity, novelty, and citation count @li2019correlation. Our results show that
papers with clear, concise, and novel titles tend to receive more citations than
those with longer or less informative titles @west2013role. Moreover, we found
that papers with creative and attention-grabbing titles tend to attract more
readers and citations, which supports our hypothesis that the title alone has
the highest impact on citation count. Our findings suggest that researchers
should pay more attention to crafting effective titles that accurately and
creatively summarize the main message of their research, as it can have a
significant impact on the success and visibility of their work.

= Declaration of Interest Statement
#label("declaration-of-interest-statement")

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

== subsection

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

=== subsubection

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

The authors report there are no competing interests to declare.

= Introduction

#env("Theorem", name: "name")[] this
#env("Theorem", name: "name")[this]
#env("Theorem")[this]
#env("Theorem")[this]

Title of a scientific paper is an important element @teixeira2015importance that
conveys the main message of the study to the readers @hartley2019academic. In
this study, we investigate the impact of paper titles on citation count, and
propose that the title alone has the highest impact on citation count.

Using a dataset of 1000 scientific papers from various disciplines, we analyzed
the correlation between the characteristics of paper titles, such as length,
clarity, novelty, and citation count @li2019correlation. Our results show that
papers with clear, concise, and novel titles tend to receive more citations than
those with longer or less informative titles @west2013role. Moreover, we found
that papers with creative and attention-grabbing titles tend to attract more
readers and citations, which supports our hypothesis that the title alone has
the highest impact on citation count. Our findings suggest that researchers
should pay more attention to crafting effective titles that accurately and
creatively summarize the main message of their research, as it can have a
significant impact on the success and visibility of their work.

= Declaration of Interest Statement
#label("declaration-of-interest-statement")

== subsection

=== subsubection

The authors report there are no competing interests to declare.

= Introduction

Title of a scientific paper is an important element @teixeira2015importance that
conveys the main message of the study to the readers @hartley2019academic. In
this study, we investigate the impact of paper titles on citation count, and
propose that the title alone has the highest impact on citation count.

Using a dataset of 1000 scientific papers from various disciplines, we analyzed
the correlation between the characteristics of paper titles, such as length,
clarity, novelty, and citation count @li2019correlation. Our results show that
papers with clear, concise, and novel titles tend to receive more citations than
those with longer or less informative titles @west2013role. Moreover, we found
that papers with creative and attention-grabbing titles tend to attract more
readers and citations, which supports our hypothesis that the title alone has
the highest impact on citation count. Our findings suggest that researchers
should pay more attention to crafting effective titles that accurately and
creatively summarize the main message of their research, as it can have a
significant impact on the success and visibility of their work.

= Declaration of Interest Statement
#label("declaration-of-interest-statement")

== subsection

=== subsubection

The authors report there are no competing interests to declare.

= Introduction

Title of a scientific paper is an important element @teixeira2015importance that
conveys the main message of the study to the readers @hartley2019academic. In
this study, we investigate the impact of paper titles on citation count, and
propose that the title alone has the highest impact on citation count.

Using a dataset of 1000 scientific papers from various disciplines, we analyzed
the correlation between the characteristics of paper titles, such as length,
clarity, novelty, and citation count @li2019correlation. Our results show that
papers with clear, concise, and novel titles tend to receive more citations than
those with longer or less informative titles @west2013role. Moreover, we found
that papers with creative and attention-grabbing titles tend to attract more
readers and citations, which supports our hypothesis that the title alone has
the highest impact on citation count. Our findings suggest that researchers
should pay more attention to crafting effective titles that accurately and
creatively summarize the main message of their research, as it can have a
significant impact on the success and visibility of their work.

= Declaration of Interest Statement
#label("declaration-of-interest-statement")

== subsection

=== subsubection

= Introduction

= Introduction

= Introduction

= Introduction


The authors report there are no competing interests to declare.
