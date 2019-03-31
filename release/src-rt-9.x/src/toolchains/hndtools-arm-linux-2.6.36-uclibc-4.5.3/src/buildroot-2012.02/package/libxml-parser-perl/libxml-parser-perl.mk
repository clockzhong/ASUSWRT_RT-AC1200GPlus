#############################################################
#
# libxml-parser-perl
#
#############################################################
LIBXML_PARSER_PERL_VERSION:=2.36
LIBXML_PARSER_PERL_SOURCE:=XML-Parser-$(LIBXML_PARSER_PERL_VERSION).tar.gz
LIBXML_PARSER_PERL_SITE:=http://www.cpan.org/modules/by-module/XML/

LIBXML_PARSER_PERL_DEPENDENCIES = expat

define HOST_LIBXML_PARSER_PERL_CONFIGURE_CMDS
 (cd $(@D) ; \
   perl Makefile.PL \
        PREFIX=$(TOOLCHAINS_DIR) \
        EXPATLIBPATH=$(TOOLCHAINS_DIR)/lib \
        EXPATINCPATH=$(TOOLCHAINS_DIR)/include \
        INSTALLDIRS=site \
        INSTALLSITELIB=$(TOOLCHAINS_DIR)/lib/perl \
        INSTALLSITEARCH=$(TOOLCHAINS_DIR)/lib/perl \
 )
endef

define HOST_LIBXML_PARSER_PERL_BUILD_CMDS
 $(HOST_MAKE_ENV) $(MAKE) -C $(@D)
endef

define HOST_LIBXML_PARSER_PERL_INSTALL_CMDS
 $(HOST_MAKE_ENV) $(MAKE) -C $(@D) install
endef

$(eval $(call GENTARGETS))
$(eval $(call GENTARGETS,host))

