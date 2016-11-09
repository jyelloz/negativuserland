NULL =

ASCIIDOCTOR = bundle exec asciidoctor
ASCIIDOCTOR_PDF = bundle exec asciidoctor-pdf

ASCIIDOCTOR_COMMON_FLAGS = -d book
ASCIIDOCTOR_FLAGS =
ASCIIDOCTOR_PDF_FLAGS = \
	-a pdf-stylesdir=resources/themes \
	-a pdf-style=csunthesis \
	$(NULL)


ASCIIDOCTOR_STAMP = vendor/bundle/ruby/2.3.0/bin/asciidoctor

SOURCES = thesis.adoc \
		  abstract.adoc \
		  chapter1/chapter1.adoc \
		  chapter2/chapter2.adoc \
		  chapter3/chapter3.adoc \
		  chapter4/chapter4.adoc \
		  bibliography.adoc \
		  $(NULL)

all: thesis.html thesis.pdf

bootstrap: $(ASCIIDOCTOR_STAMP) Makefile

$(ASCIIDOCTOR_STAMP):
	bundle install --path vendor/bundle

thesis.html: $(SOURCES) bootstrap
	$(ASCIIDOCTOR) $(ASCIIDOCTOR_COMMON_FLAGS) $(ASCIIDOCTOR_FLAGS) $<

thesis.pdf: $(SOURCES) resources/themes/csunthesis-theme.yml bootstrap
	$(ASCIIDOCTOR_PDF) $(ASCIIDOCTOR_COMMON_FLAGS) $(ASCIIDOCTOR_PDF_FLAGS) $<

clean:
	rm -f thesis.html thesis.pdf

distclean:
	rm -fR vendor

.PHONY = all clean distclean bootstrap
