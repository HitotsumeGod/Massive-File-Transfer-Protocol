CC=gcc
SRC=src/main
BUILD=build

all: $(BUILD)/server $(BUILD)/client $(BUILD)/myftp
clean:
	rm -rf $(BUILD)
$(BUILD)/server: $(SRC)/server.c
	if [ ! -d $(BUILD) ]; then \
		mkdir build; \
	fi
	$(CC) -o $@ $^ 
$(BUILD)/client: $(SRC)/client.c
	if [ ! -d $(BUILD) ]; then \
		mkdir $(BUILD); \
	fi
	$(CC) -o $@ $^
$(BUILD)/myftp: $(SRC)/myftp.c
	if [ ! -d $(BUILD) ]; then \
		mkdir $(BUILD); \
	fi
	$(CC) -o $@ $^
