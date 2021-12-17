CXXFLAGS += -Wall -Werror -O2

SRCS=cpu_usage.cpp example.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

EXAMPLE=cpu_usage

all: $(EXAMPLE)

$(EXAMPLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXAMPLE)

clean:
	rm -f $(OBJS) $(EXAMPLE)
