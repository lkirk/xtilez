CFLAGS := -Wall -Wextra -Wpedantic
X11-LIBS := $(shell pkg-config x11 --libs)

BIN := xtilez

$(BIN): *.c *.h
	clang -g $(CFLAGS) $(X11-LIBS) $(filter-out %.h,$(^)) -o ${@}

_xephyr-server:
	nohup Xephyr -noreset -screen 800x600 :2 2>&1 | tee /dev/null &

manual-test: _xephyr-server
	sleep .5; DISPLAY=:2 ./xtilez

debug: _xephyr-server
	DISPLAY=:2 gdb ./xtilez

.PHONY: debug manual-test _xephyr-server
