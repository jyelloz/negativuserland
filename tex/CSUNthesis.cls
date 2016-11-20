%%
%% This is file `CSUNthesis.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% CSUNthesis.dtx  (with options: `class')
%% 
%% Copyright (C) 2004 by Jeff Wiegley, Ph.D.
%% 
%% This file may be distributed and/or modified under the
%% conditions of the LaTeX Project Public License, either
%% version 1.2 or this license of (at your option) any
%% later version. The latest version of this license is in:
%% 
%%    http://www.latex-project.org/lppl.txt
%% 
%% and version 1.2 or later is part of all distributions of
%% LaTeX version 1999/12/01 or later.
%% 
%%% The todo list:
%%% Tips and tricks
%%% spacing ties ~
%%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{CSUNthesis}
    [2006/04/26 2.2 CSU Northridge]
%%most CS students will want to present code listings.
%%most authors will want to include graphics
%% There is more to life than computers
%%various front matter pages, the absence of any of these
%%macros just causes an absence of those pages.
%%information for producing a defense announcement page
%%comment out to supress announcement page.
%%Cause a copyright page to appear after the title page.
%%references come before appendicies.
 % \begin{quote}
\typeout{-- CSUN Thesis style} \typeout{-- Author: Wiegley, Jeff,
jeffw@csun.edu}
\typeout{--   Please contact the author to have any mistakes corrected.}
\usepackage{times}
\newlength{\titlewidth}
\setlength{\titlewidth}{4.5in}
\newlength{\frontmattertopmargin}
\setlength{\frontmattertopmargin}{1.0in}

\newif\ifproposal\proposalfalse
\newif\ifthesis\thesisfalse
\newif\ifabstract\abstractfalse
\newif\ifproject\projectfalse
\newif\iflof\loffalse
\newif\iflot\lotfalse
\newif\iflol\lotfalse
\newif\ifdraft\draftfalse
\newif\ifsizespec\sizespecfalse
\newif\ifmadebib\madebibfalse
\DeclareOption{twocolumn}{
  \OptionNotUsed
  \ClassError{CSUNthesis}{only single column documents allowed}{}
}
\DeclareOption{twosided}{
  \OptionNotUsed
  \ClassError{CSUNthesis}{only single sided publications allowed}{}
}
\DeclareOption{8pt}{
  \OptionNotUsed
  \ClassError{CSUNthesis}{only 10pt or 12pt typeface allowed}{}
}
\DeclareOption{9pt}{
  \OptionNotUsed
  \ClassError{CSUNthesis}{only 10pt or 12pt typeface allowed}{}
}
\DeclareOption{10pt}{
  \sizespectrue
  \PassOptionsToClass{\CurrentOption}{report}
}
\DeclareOption{11pt}{
  \sizespectrue
  \PassOptionsToClass{\CurrentOption}{report}
}
\DeclareOption{12pt}{
  \sizespectrue
  \PassOptionsToClass{\CurrentOption}{report}
}
\DeclareOption{a4paper}{
  \OptionNotUsed
  \ClassWarning{CSUNthesis}{CSU requires letter sized paper, a4paper
    ignored}{}
}
\DeclareOption{thesis}{
  \ifproject
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \ifabstract
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \thesistrue
}
\DeclareOption{project}{
  \ifthesis
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \ifabstract
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \projecttrue
}
\DeclareOption{abstract}{
  \ifthesis
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \ifproject
  \ClassError{CSUNthesis}{only one thesis type may be specified}{}
  \fi
  \abstracttrue
}

\DeclareOption{lof}{\loftrue}
\DeclareOption{nolof}{\loftrue}
\DeclareOption{lol}{\loltrue}
\DeclareOption{nolol}{\loltrue}
\DeclareOption{lot}{\lottrue}
\DeclareOption{nolot}{\lottrue}
\DeclareOption{draft}{\drafttrue\AtBeginDocument{\onehalfspacing}}
\DeclareOption{draftcls}{\drafttrue\AtBeginDocument{\onehalfspacing}}
\DeclareOption{final}{\draftfalse\AtBeginDocument{\singlespacing}}
\DeclareOption{proposal}{\proposaltrue}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ExecuteOptions{final}
\ProcessOptions\relax
\ifsizespec\relax\else\PassOptionsToClass{12pt}{report}\fi
\LoadClass{report}
\usepackage[usenames]{color}
\usepackage{ulem}
\normalem
\newcommand{\udot}{\bgroup \markoverwith{\lower .4ex\hbox{.}}\ULon}
\newcommand{\udash}{\bgroup \markoverwith{\lower .8ex\hbox{-}}\ULon}
\newcommand{\marginreason}[2]{%
\def\thereason{#2}%
\ifx\@empty\thereason\relax\else\marginpar{\color{#1}#2}\fi%
}
\newcommand\edit[2]{%
  \typeout{WARNING: edit change tracking macro exists in source}%
  {\color{Red}\uline{#1}}\marginreason{Red}{#2}}%
\newcommand\delete[2]{%
  \typeout{WARNING: delete change tracking macro exists in source}%
  {\color{Red}\sout{#1}}\marginreason{Red}{#2}}%
\newcommand\add[2]{%
  \typeout{WARNING: add change tracking macro exists in source}%
  {\color{Red}\udot{#1}}\marginreason{Red}{#2}}%
%%    \begin{macrocode}
\newcommand\replace[3]{%
  \typeout{WARNING: replace change tracking macro exists in source}%
  {\color{Red}\sout{#1}\udot{#2}}\marginreason{Red}{#3}}%
%%    \begin{macrocode}
\newcommand\note[1]{%
  \typeout{WARNING: replace change tracking macro exists in source}%
  {\color{Red}\uwave{#1}}}%

\renewcommand\contentsname{\normalfont Table of Contents}
\renewcommand\bibname{References}
\def\@makechapterhead#1{%
  {\parindent \z@ \centering \normalfont
    \ifnum \c@secnumdepth >\m@ne
        \bfseries\boldmath \@chapapp\space \thechapter
        \par\nobreak
    \fi
    \interlinepenalty\@M
    \bfseries\boldmath #1\par\nobreak
    \vskip 20\p@
  }}

\def\@makeschapterhead#1{%
  {\parindent \z@ \centering
    \normalfont
    \interlinepenalty\@M
    \bfseries\boldmath #1\par\nobreak
    \vskip 20\p@
  }}

\def\@makepchapterhead#1{%
  {\parindent \z@ \centering
    \normalfont
    \interlinepenalty\@M
    #1\par\nobreak
    \vskip 20\p@
  }}

\def\prefacechapter#1{\if@twocolumn
                   \@topnewpage[\@makepchapterhead{#1}]%
                 \else
                   \@makepchapterhead{#1}%
                   \@afterheading
                 \fi}

\renewcommand\listoffigures{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \prefacechapter{\listfigurename}%
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}%
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
    }

\renewcommand\listoftables{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \prefacechapter{\listtablename}%
      \@mkboth{%
          \MakeUppercase\listtablename}%
         {\MakeUppercase\listtablename}%
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
    }

\renewcommand{\section}{
  \@startsection{section}{1}{0mm}{0pt}{0.0001pt}%
{\noindent\normalfont\normalsize\bfseries\boldmath}}%

\renewcommand{\subsection}{
  \@startsection{subsection}{1}{0mm}{0pt}{0.0001pt}%
{\noindent\normalfont\normalsize\bfseries\boldmath}}%

\renewcommand{\subsubsection}{
  \@startsection{subsubsection}{1}{0mm}{0pt}{0.0001pt}%
{\noindent\normalfont\normalsize\bfseries\boldmath}}%

\renewcommand*\l@chapter[2]{%
  \ifnum \c@tocdepth >\m@ne%
    \addpenalty{-\@highpenalty}%
    \vskip 1.0em \@plus\p@%
    \setlength\@tempdima{1.5em}%
    \begingroup%
      \parindent \z@ \rightskip \@pnumwidth%
      \parfillskip -\@pnumwidth%
      \leavevmode%\bfseries
      \advance\leftskip\@tempdima%
      \hskip -\leftskip%
      #1\nobreak\hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par%
      \penalty\@highpenalty%
    \endgroup%
  \fi}

\renewcommand{\maketitle}{}

\ifproposal
\renewcommand{\thesection}{\arabic{section}}
\fi

\ifthesis
\relax
\else
\ifabstract\relax\else\ifproject\relax\else\thesistrue\fi\fi
\fi

\setlength{\headheight}{0.0in}     % results in 1.0inch
\setlength{\headsep}{0.0in}     % results in 1.0inch
\setlength{\topmargin}{0.0in}     % results in 1.0inch
\setlength{\textheight}{9.0in}
\setlength{\footskip}{0.5in}
\setlength{\oddsidemargin}{0.5in} % results in 1.5in
\setlength{\evensidemargin}{\oddsidemargin}
\setlength{\textwidth}{6.0in}

\RequirePackage{setspace}

\pagestyle{plain}

\newcommand{\@degree}{Master~of~Science}

\newcommand{\@department}{Computer~Science}

\newcommand{\degree}[2]{
  \renewcommand{\@degree}{#1}
  \renewcommand{\@department}{#2}
}


\newcommand{\references}[2]{
  \ifx\undefined\@thesisbibstyle
  \newcommand{\@thesisbibstyle}{#1}
  \else
  \renewcommand{\@thesisbibstyle}{#1}
  \fi
  \ifx\undefined\@thesisbibfile
  \newcommand{\@thesisbibfile}{#2}
  \else
  \renewcommand{\@thesisbibfile}{#2}
  \fi
}

\newcommand{\submitted}[2]{
  \ifx\undefined\@submitmonth
  \newcommand{\@submitmonth}{#1}
  \else
  \renewcommand{\@submitmonth}{#1}
  \fi
  \ifx\undefined\@submityear
  \newcommand{\@submityear}{#2}
  \else
  \renewcommand{\@submityear}{#2}
  \fi
}

\newcommand{\defense}[4]{
  \ifx\undefined\@defenseday
  \newcommand{\@defenseday}{#1}
  \newcommand{\@defensedate}{#2}
  \newcommand{\@defensetime}{#3}
  \newcommand{\@defenselocation}{#4}
  \else
  \renewcommand{\@defenseday}{#1}
  \renewcommand{\@defensedate}{#2}
  \renewcommand{\@defensetime}{#3}
  \renewcommand{\@defenselocation}{#4}
  \fi
}

\newcommand{\contact}[1]{
  \ifx\undefined\@contact
  \newcommand{\@contact}{#1}
  \else
  \renewcommand{\@contact}{#1}
  \fi
}

\newcommand{\collaboration}[1]{
  \ifx\undefined\@collaborator
  \newcommand{\@collaborator}{#1}
  \else
  \renewcommand{\@collaborator}{#1}
  \fi
}

\newcommand{\dedication}[1]{
  \ifx\undefined\@dedication
  \newcommand{\@dedication}{#1}
  \else
  \renewcommand{\@dedication}{#1}
  \fi
}

\newcommand{\acknowledgement}[1]{
  \ifx\undefined\@acknowledgement
  \newcommand{\@acknowledgement}{#1}
  \else
  \renewcommand{\@acknowledgement}{#1}
  \fi
}

\newcommand{\preface}[1]{
  \ifx\undefined\@preface
  \newcommand{\@preface}{#1}
  \else
  \renewcommand{\@preface}{#1}
  \fi
}

\renewcommand{\abstract}[1]{
  \ifx\undefined\@abstract
  \newcommand{\@abstract}{#1}
  \else
  \renewcommand{\@abstract}{#1}
  \fi
}

\newcommand{\copyrightyear}[1]{
  \ifx\undefined\@copyrightyear
  \newcommand{\@copyrightyear}{#1}
  \else
  \renewcommand{\@copyrightyear}{#1}
  \fi
}

\newcommand{\coordinator}[1]{
  \ifx\undefined\@coordinator
  \newcommand{\@coordinator}{#1}
  \else
  \renewcommand{\@coordinator}{#1}
  \fi
}

\newcommand{\committee}[3]{
  \ifx\undefined\@memberA
  \newcommand{\@memberA}{#2}
  \else
  \renewcommand{\@memberA}{#2}
  \fi
  \ifx\undefined\@memberB
  \newcommand{\@memberB}{#3}
  \else
  \renewcommand{\@memberB}{#3}
  \fi
  \ifx\undefined\@memberChair
  \newcommand{\@memberChair}{#1}
  \else
  \renewcommand{\@memberChair}{#1}
  \fi
}

\newcommand{\frontpagesetup}[1]{
  \begin{center}
    %\Large #1
    #1
  \end{center}
}

\newcommand{\mpbibliography}{
  \ifmadebib\relax\else
  \ifx\undefined\@thesisbibstyle
  \typeout{WARNING: YOU NEED A BIBLIOGRAPHY!! See the references macro}
  \else
  \bibliographystyle{\@thesisbibstyle}
  \bibliography{\@thesisbibfile}
  \addcontentsline{toc}{chapter}{\bibname}
  \madebibtrue\fi
  \fi
}

\renewcommand\appendix{\mpbibliography\par
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \gdef\@chapapp{\appendixname}%
  \gdef\thechapter{\@Alph\c@chapter}}

\newcommand{\mpproposal}{
  \ifx\undefined\@author
  \ClassError{CSUNthesis}{no \protect\author{<author>} given.}{}
  \fi
  \ifx\undefined\@title
  \ClassError{CSUNthesis}{no \protect\title{<title>} given.}{}
  \fi
  {
    \thispagestyle{empty}
    \begin{center}
      \vspace*{\frontmattertopmargin}
      %University heading
      CALIFORNIA STATE UNIVERSITY, NORTHRIDGE

      %space before title
      \vspace{0.625in}

      %title
      %minipage required because the title should wrap rather narrow
      \begin{minipage}{5.5in}
\centering
\begin{spacing}{0.0}

  \textbf{\MakeUppercase{\@title}}

\end{spacing}

\vspace{0.375in}

\begin{spacing}{3.0}

  \ifthesis A thesis
          \else
             \ifproject A graduate project
             \else An abstract
             \fi
          \fi
  proposal for the degree of
  \ifx\undefined\@degree
    Master~of~Science
  \else
    \@degree
  \fi
  \ in
  \ifx\undefined\@department
    Computer~Science
  \else
    \@department
  \fi

  \vspace{0.375in}

  By

  \vspace{0.375in}

  \@author
  \ifx\undefined\@contact
  \relax
  \else
  \\\@contact
  \fi

  \today

  \ifx\undefined\@collaborator
  \relax
  \else
  \vspace*{0.625in}
  in collaboration with\\\@collaborator
  \fi
\end{spacing}
      \end{minipage}

      % the date needs to be outside of the minipage so that \vfill
      % works to flush the date to the bottom of the page.

      \vfill
      \hfill
      \begin{minipage}{4.00in}

\makebox[2.75in]{\hrulefill}\makebox[0.5in][r]{Date:}
\hrulefill\\
\makebox[4.125in][l]{\@memberChair, Committee Chair}

\vspace{.25in}

\makebox[2.75in]{\hrulefill}\makebox[0.5in][r]{Date:}
\hrulefill\\
\makebox[4.125in][l]{\@coordinator, Graduate Coordinator}

\vspace{.25in}

\makebox[2.75in]{\hrulefill}\makebox[0.5in][r]{Date:}
\hrulefill\\
\makebox[4.125in][l]{\@author, Student}

      \end{minipage}
      %fill to the bottom of page
    \end{center}
  }
  \newpage
}

\newcommand{\mpannouncement}
{
  \ifx\undefined\@defensedate
  \relax
  \else
  \thispagestyle{empty}
  % save current margin information
  \newlength{\oldwidth}
  \newlength{\oldoddside}
  \newlength{\oldtop}
  \newlength{\oldheight}
  \setlength{\oldtop}{\topmargin}
  \setlength{\oldheight}{\textheight}
  \setlength{\oldwidth}{\textwidth}
  \setlength{\oldoddside}{\oddsidemargin}
  % set new margins for even page layout
  \setlength{\oddsidemargin}{0.25in}
  \setlength{\textwidth}{6in}
  \setlength{\topmargin}{0.25in}
  \setlength{\textheight}{8.5in}
  % draw the simple border frame
  % picture width and height needs to be (0,0) so as not
  % to take up space. (you can draw outside the boundary
  \noindent\begin{picture}(0,0)(72,-81.5)
  \put(0,0){\line(1,0){576}}
  \put(0,0){\line(0,-1){756}}
  \put(576,-756){\line(-1,0){576}}
  \put(576,-756){\line(0,1){756}}
  \end{picture}
  \vspace{-.5in}
  \begin{center}
    \bfseries
    \begin{minipage}{5in}
      \begin{center}
MASTERS PRESENTATION

\vspace{0.5in}

\MakeUppercase{\@title}

By

\@author
      \end{center}

      \noindent Committee Members:
      \begin{list}{}{\leftmargin=2in\itemsep=-6pt\topsep=-6pt}
      \item \@memberChair\ (Chair)
      \item \@memberA
      \item \@memberB
      \end{list}

      \vspace{14pt}
      \noindent\begin{tabular}{@{}ll}
      Date: & \@defenseday, \@defensedate\ at \@defensetime \\
      Location: & \@defenselocation
      \end{tabular}
    \end{minipage}
  \end{center}
  \begin{center}
    \bfseries ABSTRACT
  \end{center}
  \noindent \@abstract

  % generate the page
  \newpage
  %reset margins to original values
  \setlength{\oddsidemargin}{\oldoddside}
  \setlength{\textwidth}{\oldwidth}
  \setlength{\textheight}{\oldheight}
  \setlength{\topmargin}{\oldtop}
  \fi
}

\newcommand{\mptitle}{
  \ifx\undefined\@submitmonth
  \ClassError{CSUNthesis}{no
    \protect\submitted{<month>}{<year>}given.}{}
  \fi
  \ifx\undefined\@submityear
  \ClassError{CSUNthesis}{no
    \protect\submitted{<month>}{<year>}given.}{}
  \fi
  \ifx\undefined\@author
  \ClassError{CSUNthesis}{no \protect\author{<author>} given.}{}
  \fi
  \ifx\undefined\@title
  \ClassError{CSUNthesis}{no \protect\title{<title>} given.}{}
  \fi
  {
    \thispagestyle{empty}
    \setcounter{page}{1}
    \begin{center}
      %University heading
      CALIFORNIA STATE UNIVERSITY, NORTHRIDGE

      %space before title
      \vspace{2.625in}

      %title
      %minipage required because the title should wrap rather narrow
      \begin{minipage}{\titlewidth}
\centering
\begin{spacing}{2.0}

  \MakeUppercase{\@title}

\end{spacing}

\vspace{0.375in}

\begin{spacing}{1.0}

  \ifthesis A thesis
          \else
             \ifproject A graduate project
             \else An abstract
             \fi
          \fi
  submitted in partial fulfillment of the
  requirements for the degree of
  \ifx\undefined\@degree
    Master~of~Science
  \else
    \@degree\
  \fi
  in
  \ifx\undefined\@department
    Computer~Science
  \else
    \@department
  \fi

  \vspace{0.375in}

  By

  \vspace{0.375in}

  \@author
  \ifdraft
  \ifx\undefined\@contact
  \relax
  \else
  \\\@contact
  \fi
  \fi

  \ifx\undefined\@collaborator
  \relax
  \else
  \vspace*{0.625in}
  in collaboration with\\\@collaborator
  \fi
\end{spacing}
      \end{minipage}

      % the date needs to be outside of the minipage so that \vfill
      % works to flush the date to the bottom of the page.

      %fill to the bottom of page
      \vfill


      \@submitmonth\ \@submityear
    \end{center}
  }
  \newpage
}

\newcommand{\mpcopyright}{
  \ifx\undefined\@copyrightyear
    \relax
  \else
  \null\vfill
  \begin{center}
    %\fontsize{14}{16.8}\selectfont
    \copyright\ Copyright\ by \@author\ \@copyrightyear\\
    All Rights Reserved
  \end{center}
  \addcontentsline{toc}{chapter}{Copyright}
  \newpage
  \fi
}

\newcommand{\mpsignature}{
  \ifx\undefined\@memberA
  \ClassError{CSUNthesis}{No
    \protect\committee given; must be provided}{}
  \fi
  \ifx\undefined\@memberB
  \ClassError{CSUNthesis}{No
    \protect\committee given; must be provided}{}
  \fi
  \ifx\undefined\@memberChair
  \ClassError{CSUNthesis}{No
    \protect\committee given; must be provided}{}
  \fi
  \ifx\undefined\@author
  \ClassError{CSUNthesis}{no \protect\author{<author>} given.}{}
  \fi
  \vspace*{2.5in}
  \begin{center}
    \begin{minipage}{5.05in}
      The
      \ifthesis
      thesis
      \else
      \ifproject
      graduate project
      \else
      abstract
      \fi
      \fi
      of \@author\ is approved:

      \vspace*{1in}

      \makebox[3in]{\hrulefill}\makebox[0.5in]{}
      \makebox[1.5in]{\hrulefill}\\
      \makebox[3in][l]{\@memberA}\makebox[0.5in]{}
      \makebox[1.5in][l]{Date}

      \vspace{.375in}

      \makebox[3in]{\hrulefill}\makebox[0.5in]{}
      \makebox[1.5in]{\hrulefill}\\
      \makebox[3in][l]{\@memberB}\makebox[0.5in]{}
      \makebox[1.5in][l]{Date}

      \vspace{.375in}

      \makebox[3in]{\hrulefill}\makebox[0.5in]{}
      \makebox[1.5in]{\hrulefill}\\
      \makebox[3in][l]{\@memberChair, Chair}\makebox[0.5in]{}
      \makebox[1.5in][l]{Date}

    \end{minipage}
    \vfill
    California State University, Northridge
  \end{center}
  \addcontentsline{toc}{chapter}{Signature page}
  \newpage
}

\newcommand{\mppreface}{
  \ifx\undefined\@preface
    \relax
  \else
    \frontpagesetup{Preface}
    \@preface
    \addcontentsline{toc}{chapter}{Preface}
    \newpage
  \fi
}

\newcommand{\mpdedication}{
  \ifx\undefined\@dedication
    \relax
  \else
    \frontpagesetup{Dedication}
    \@dedication
    \addcontentsline{toc}{chapter}{Dedication}
    \newpage
  \fi
}

\newcommand{\mpacknowledgement}{
  \ifx\undefined\@acknowledgement
    \relax
  \else
    \frontpagesetup{Acknowledgements}
    \@acknowledgement
    \addcontentsline{toc}{chapter}{Acknowledgements}
    \newpage
  \fi
}

\newcommand{\mptableofcontents}{
%%  \newlength{\oldparskip}
%%  \setlength{\oldparskip}{\parskip}
%%  \setlength{\parskip}{0pt}
%%Graduate Evaluator Shrew, now changes her mind, again, and this
%%is no longer allowed to be listed in the Table of Contents;
%%Contrary to her original whim which made me put this in here in
%%the first place.
%%  \addcontentsline{toc}{chapter}{Table of Contents}
  \tableofcontents
%%  \setlength{\parskip}{\oldparskip}
  \newpage
}

\newcommand{\mplistoftables}{
  \iflot
    \addcontentsline{toc}{chapter}{List of Tables}
    \listoftables
    \newpage
  \fi
}

\newcommand{\mplistoffigures}{
  \iflof
    \addcontentsline{toc}{chapter}{List of Figures}
    \listoffigures
    \newpage
  \fi
}

\newcommand{\mplofillustrations}{}

\newcommand{\mplistoflistings}{
  \iflol
    \renewcommand{\lstlistlistingname}{\normalfont List of Listings}
    \addcontentsline{toc}{chapter}{List of Listings}
    \lstlistoflistings
    \newpage
  \fi
}

\newcommand{\mpabstract}
{
  \ifx\undefined\@abstract
    \ifproposal\relax\else
      \ClassError{CSUNthesis}
 {No \protect\abstract given; must be provided}{}
    \fi
  \fi
  {
    {
    \newlength{\oldbaselineskip}
    \setlength{\oldbaselineskip}{\baselineskip}
    \setlength{\baselineskip}{34pt}
    \begin{center}
      ABSTRACT

      \MakeUppercase{\@title}

      By

      \@author

      \@degree\ in \@department
    \end{center}
    }
    %\setlength{\baselineskip}{\oldbaselineskip}
    \@abstract
  }
  \addcontentsline{toc}{chapter}{Abstract}
  \newpage
}

\AtBeginDocument{
  \pagenumbering{roman}
  \ifproposal
  \mpproposal
  \else
  \parskip=10pt
  \mpannouncement
  \mptitle
  \mpcopyright
  \mpsignature
  \mpdedication
  \mpacknowledgement
  \mppreface
  \parskip=0pt
  \mptableofcontents
  \mplistoftables
  \mplistoffigures
  \mplofillustrations
  \mplistoflistings
  \parskip=10pt
  \mpabstract
  \fi
  \pagenumbering{arabic}
}

\AtEndDocument{%
\mpbibliography%
}

\endinput
%%
%% End of file `CSUNthesis.cls'.
