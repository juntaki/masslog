subdirs := lib collector test
.PHONY: all $(subdirs)
all: $(subdirs)
$(subdirs):
	$(MAKE) -C $@
