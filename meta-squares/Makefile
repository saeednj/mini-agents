SRCS=main.cpp meta.cpp util.cpp user.cpp
OBJS=$(SRCS:.cpp=.o)


all: $(OBJS)
	g++ -o main $(OBJS)

.cpp.o: $(SRCS)
	g++ -c $< -o $@ -O2


clean:
	rm -f $(OBJS) main
