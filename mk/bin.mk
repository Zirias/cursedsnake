define BINRULES
BINARIES += $$(BINDIR)$$(PSEP)$(T)$$(EXE)
SOURCES += $$($(T)_SOURCES)

$$(BINDIR)$$(PSEP)$(T)$$(EXE): \
    $$($(T)_SOURCES:.c=.o) $$($(T)_LIBS) | $$($(T)_DEPS) bindir
	$$(VLD)
	$$(VR)$$(CC) -o$$@ $$(LDFLAGS) $$^

$(P)%.d: $(P)%.c Makefile conf.mk mk$(PSEP)bin.mk $$($(T)_DEPS)
	$$(VDEP)
	$$(VR)$$(CCDEP) -MT"$@ $(@:.d=.o) $(@:.d=_s.o)" -MF$$@ \
	    $$(CFLAGS) $$(INCLUDES) $$<

ifneq ($$(MAKECMDGOALS),clean)
ifneq ($$(MAKECMDGOALS),distclean)
-include $$($(T)_SOURCES:.c=.d)
endif
endif

$(P)%.o: $(P)%.c Makefile conf.mk mk$(PSEP)bin.mk $$($(T)_DEPS)
	$$(VCC)
	$$(VR)$$(CC) -o$$@ -c $$(CFLAGS) $$($(T)_DEFINES) \
	    $$(INCLUDES) $$($(T)_INCLUDES) $$<
endef
