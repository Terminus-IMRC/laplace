PROG=laplace
SRCS=main.c laplace.c field.c
SRCS_MNDEP=laplace.c field.c
TARGETS=all $(PROG) %.o %.d run line clean mpg
NONEED_DEP_TARGETS+=clean line mpg
FPS:=25

OBJS=$(SRCS:%.c=%.c.o)
OBJS_MNDEP=$(SRCS_MNDEP:%.c=%.c.o)
DEPS=$(SRCS:%.c=%.c.d)
ALLDEP=$(MAKEFILE_LIST_SANS_DEPS)
TOCLEAN=out_*.png out.mpg

all: $(PROG)

.PHONY: mpg
mpg:
	ffmpeg -y -v quiet -qscale 1 -r $(FPS) -i out_%d.png out.mpg

EXTRA_TARGETS=$(filter-out $(TARGETS), $(MAKECMDGOALS))
ifneq '$(EXTRA_TARGETS)' ''
 $(error No rule to make target `$(word 1, $(EXTRA_TARGETS))')
else
 # check whether NONEED_DEP_TARGETS are in MAKECMDGOALS
 ifeq '$(filter-out $(NONEED_DEP_TARGETS), $(MAKECMDGOALS))' '$(MAKECMDGOALS)'
  sinclude $(DEPS)
 else
  # if so and there are more than 1 targets in MAKECMDGOALS, it would cause dependency files missing so say error
  ifneq '$(words $(MAKECMDGOALS))' '1'
   $(error Specify only one target if you want to make target that needs no dependency file)
  endif
 endif
endif

MAKEFILE_LIST_SANS_DEPS=$(filter-out %.d, $(MAKEFILE_LIST))

CC:=cc
HEADERFLAGS:=-I.
OPTFLAGS:=-pipe -O0 -g
WARNFLAGS:=-ansi -pedantic -Wall -Wextra -W -Wundef -Wshadow -Wcast-qual -Winline -Wno-long-long -fsigned-char
ADDCFLAGS+=-fopenmp
LINKFLAGS+=-lpng -fopenmp
RM:=rm -r -f
WC:=wc -c -l

COMPILE.c=$(CC) $(HEADERFLAGS) $(OPTFLAGS) $(WARNFLAGS) $(DEPFLAGS) $(ADDCFLAGS) $(CFLAGS) -c
COMPILE.dep=$(CC) $(HEADERFLAGS) $(OPTFLAGS) $(WARNFLAGS) $(DEPFLAGS) $(ADDCFLAGS) $(CFLAGS) -M -MP -MT $<.o -MF $@
LINK.o=$(CC) $(OPTFLAGS) $(WARNFLAGS) $(LINKFLAGS) $(LDFLAGS)

# $(call set-dep-val, val-name, default-val, dep-obj-file-list)
define set-dep-val
 $1:=$2
 $(strip $1)NUM:=.$(strip $1)
 $$(shell ([ -f $$($(strip $1)NUM) ] && [ `cat $$($(strip $1)NUM)` -eq "$$($(strip $1))" ]) || echo $$($(strip $1)) >$$($(strip $1)NUM))
 $3: $$($(strip $1)NUM)
 $3: DEPFLAGS+=-D$(strip $1)=$$($(strip $1))
endef

$(eval $(call set-dep-val, M, 100, $(OBJS_MNDEP)))
$(eval $(call set-dep-val, N, 100, $(OBJS_MNDEP)))

$(PROG): $(OBJS) $(ALLDEP)
	$(LINK.o) $(OUTPUT_OPTION) $(OBJS)

%.c.o: %.c $(ALLDEP)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.c.d: %.c $(ALLDEP)
	$(COMPILE.dep) $<

run: $(PROG)
	./$(PROG)

.PHONY: line
line:
	$(WC) $(SRCS) $(MAKEFILE_LIST_SANS_DEPS)

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(MNUM) $(NNUM)
	$(RM) $(TOCLEAN)
