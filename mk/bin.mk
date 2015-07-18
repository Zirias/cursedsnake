define BINRULES
BINARIES += $$(BINDIR)$$(PSEP)$(T)$$(EXE)

$(T)_SOURCES_FULL := $$(addprefix $(P)$$(PSEP),$$($(T)_SOURCES))
ifneq ($$(strip $$($(T)_PLATFORMSOURCES)),)
$(T)_SOURCES_FULL += $$(addprefix \
	$(P)$$(PSEP)platform$$(PSEP)$$(PLATFORM)$$(PSEP), \
	$$($(T)_PLATFORMSOURCES))
endif
SOURCES += $$($(T)_SOURCES_FULL)

$$(BINDIR)$$(PSEP)$(T)$$(EXE): $$($(T)_SOURCES_FULL:.c=.o) \
    $$($(T)_LIBS) | bindir
	$$(VLD)
	$$(VR)$$(CC) -o$$@ $$(LDFLAGS) $$($(T)_LDFLAGS) \
	       $$($(T)_$$(PLATFORM)_LDFLAGS) $$^ $$($(T)_LIBS)

$(P)$$(PSEP)%.d: $(P)$$(PSEP)%.c Makefile conf.mk
	$$(VDEP)
	$$(VR)$$(CCDEP) -MT"$@ $(@:.d=.o)" -MF$$@ \
	    $$(CFLAGS) $$(INCLUDES) $$<

ifneq ($$(MAKECMDGOALS),clean)
ifneq ($$(MAKECMDGOALS),distclean)
-include $$($(T)_SOURCES_FULL:.c=.d)
endif
endif

$(P)$$(PSEP)%.o: $(P)$$(PSEP)%.c Makefile conf.mk $(P)$$(PSEP)$(T).mk
	$$(VCC)
	$$(VR)$$(CC) -o$$@ -c $$(CFLAGS) $$($(T)_DEFINES) \
	    $$(INCLUDES) $$($(T)_INCLUDES) $$<
endef
