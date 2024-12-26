A = a b c d e f g h i j k l m n o p q r s t u v w x y z
B = $(foreach x,$(A),$(x).haha)

C = a.c b.c c.c d.c e.c f.o g.o m.c
D = $(filter %.c,$(C))
E = $(filter-out %.c,$(C))

F = $(wildcard a.c b.c e.c)

E = $(patsubst %.c,%.o,$(F))

all:
	@echo $(B)
	@echo $(D)
	@echo $(E)
	@echo $(F)
	@echo $(E)