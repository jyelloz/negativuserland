ASCIIDOCTOR = bundle exec asciidoctor
ASCIIDOCTOR_LATEX = bundle exec asciidoctor-latex

ASCIIDOCTOR_STAMP = vendor/bundle/ruby/2.3.0/bin/asciidoctor

SOURCES = thesis.adoc \
		  abstract.adoc \
		  chapter1/chapter1.adoc \
		  chapter2/chapter2.adoc \
		  chapter3/chapter3.adoc \
		  chapter4/chapter4.adoc \
		  bibliography.adoc \
		  $(NULL)

all: thesis.html thesis.tex

bootstrap: $(ASCIIDOCTOR_STAMP)

$(ASCIIDOCTOR_STAMP):
	bundle install --path vendor/bundle

thesis.html: $(SOURCES) bootstrap
	$(ASCIIDOCTOR) $<

thesis.tex: $(SOURCES) bootstrap
	$(ASCIIDOCTOR_LATEX) $<

clean:
	rm -f thesis.html thesis.tex newEnvironments.tex
	rm -fR vendor

.PHONY = all clean bootstrap
