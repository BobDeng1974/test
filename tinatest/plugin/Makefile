PLUGINS_PREFIX := CONFIG_TINATEST_SYS_GLOBAL_INFO_
MCOLLECTD_PLUGINS := $(shell \
	for one in `grep "$(PLUGINS_PREFIX)COLLECTD_[[:alnum:]]*=" $(BUILD_DIR)/.config`; \
	do \
		echo $${one} | sed 's/$(PLUGINS_PREFIX)\(.*\)=.*/m\1.so/; s/[A-Z]/\l&/g'; \
	done \
)
OUTLOG_PLUGINS := $(shell \
	for one in `grep "$(PLUGINS_PREFIX)OUTLOG_[[:alnum:]]*=" $(BUILD_DIR)/.config`; \
	do \
		echo $${one} | sed 's/$(PLUGINS_PREFIX)\(.*\)=.*/\1.so/; s/[A-Z]/\l&/g'; \
	done \
)

.PHONY: all build_plugins
all: build_plugins

build_plugins:
	@echo -e "\033[33m******** BUILD PLUGINS ********\033[0m"
ifneq ($(MCOLLECTD_PLUGINS),)
	@$(MAKE) -C ./mcollectd \
		ARCH="$(ARCH)" \
		AR="$(AR)" \
		CC="$(CC)" \
		CXX="$(CXX)" \
		CFLAGS="$(CFLAGS) -fPIC" \
		LDFLAGS="$(LDFLAGS)" \
		BUILD_DIR="$(BUILD_DIR) -shared -ltinatest" \
		PREFIX="$(PREFIX)" \
		$(MCOLLECTD_PLUGINS)
endif
ifneq ($(OUTLOG_PLUGINS),)
	@$(MAKE) -C ./outlog \
		ARCH="$(ARCH)" \
		AR="$(AR)" \
		CC="$(CC)" \
		CXX="$(CXX)" \
		CFLAGS="$(CFLAGS) -fPIC" \
		LDFLAGS="$(LDFLAGS)" \
		BUILD_DIR="$(BUILD_DIR) -shared -ltinatest" \
		PREFIX="$(PREFIX)" \
		$(OUTLOG_PLUGINS)
endif
