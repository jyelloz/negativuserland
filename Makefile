ASCIIDOCTOR = bundle exec asciidoctor
ASCIIDOCTOR_LATEX = bundle exec asciidoctor-latex


SOURCES = thesis.adoc \
		  abstract.adoc \
		  chapter1/chapter1.adoc \
		  chapter2/chapter2.adoc \
		  chapter3/chapter3.adoc \
		  chapter4/chapter4.adoc \
		  $(NULL)

all: thesis.html thesis.tex

thesis.html: $(SOURCES)
	$(ASCIIDOCTOR) $<

thesis.tex: $(SOURCES)
	$(ASCIIDOCTOR_LATEX) $<

clean:
	rm -f thesis.html thesis.tex

.PHONY = all clean
