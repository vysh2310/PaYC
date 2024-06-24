EXE=runpid
REPORT=report.tsv
OBJS=pi_app/pi_main.o \
     pi_app/pi_globals.o \
     pi_log/pi_log.o \
     pi_reset/pi_reset.o \
     pi_task/pi_task.o pi_task/pi_utils.o \
     pi_alg/pi_alg.o

INCS=-I.

CFLAGS=--std=c99 -Wall
CFLAGS+=$(INCS)
LDFLAGS=-lm -lpthread

.PHONY: report

all: $(EXE)

$(EXE): $(OBJS)
	gcc $^ $(LDFLAGS) -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(REPORT): $(EXE)
	polyspace-access -tmp-dir tmp-dir -protocol http -host muc-mbecker.dhcp.mathworks.com -port 9443 -login martin -encrypted-password IIILOFOINKJBMODNEEPHBKIHNFPFNHHO -export /public/PIControl_CP/PIControl_CP -output $(REPORT)

clean:
	rm -f $(EXE) $(OBJS)
