#############################################################
#
# python-setuptools
#
#############################################################

PYTHON_SETUPTOOLS_VERSION = 0.6c11
PYTHON_SETUPTOOLS_SOURCE  = setuptools-$(PYTHON_SETUPTOOLS_VERSION).tar.gz
PYTHON_SETUPTOOLS_SITE    = http://pypi.python.org/packages/source/s/setuptools
PYTHON_SETUPTOOLS_DEPENDENCIES = python

define HOST_PYTHON_SETUPTOOLS_BUILD_CMDS
	(cd $(@D); $(TOOLCHAINS_DIR)/bin/python setup.py build)
endef

define PYTHON_SETUPTOOLS_BUILD_CMDS
	(cd $(@D); $(TOOLCHAINS_DIR)/bin/python setup.py build)
endef

define HOST_PYTHON_SETUPTOOLS_INSTALL_CMDS
	(cd $(@D); PYTHONPATH="$(TOOLCHAINS_DIR)/lib/python$(PYTHON_VERSION_MAJOR)/site-packages"\
	$(TOOLCHAINS_DIR)/bin/python setup.py install --prefix=$(TOOLCHAINS_DIR))
endef

define PYTHON_SETUPTOOLS_INSTALL_TARGET_CMDS
	(cd $(@D); PYTHONPATH="$(TARGET_DIR)/usr/lib/python$(PYTHON_VERSION_MAJOR)/site-packages"\
	$(TOOLCHAINS_DIR)/bin/python setup.py install --prefix=$(TARGET_DIR)/usr)
endef

$(eval $(call GENTARGETS))
$(eval $(call GENTARGETS,host))

