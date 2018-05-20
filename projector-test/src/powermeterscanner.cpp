#include <projector_test/powermeterscanner.h>

#include <linux/usb/tmc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>

#include <exception>
#include <iostream>

const uint8_t MAX_CMD_LEN = 255;
const int MAX_RESP_LEN = 1024 * 1024 * 2; //Signed because sometimes the
                                          //return value is -1
const uint8_t READ_INTERVAL = 33;//Try to keep less than 33 milliseconds
// The refresh rate is 30fps. if more, values repeat

PowerMeterScanner::PowerMeterScanner(QObject *parent) :
    QObject(parent),
    timer(QTimer(this))
{
    hdrbuf = new char [MAX_RESP_LEN + 1024];
    buf = hdrbuf;
    fd = open("/dev/usbtmc0", O_RDWR);
    if(fd == -1)
    {
        delete hdrbuf;
        throw std::runtime_error("Error opening USB port for PowerMeter");
    }
//    dev_write("CONF:SCAL:POW\n");
    dev_write("SENS:AVER:COUN\\ 15"); //Each reading takes about 3ms
    connect(&timer, &QTimer::timeout, this, &PowerMeterScanner::update_reading);
    timer.setInterval(READ_INTERVAL);
    timer.start();
}

PowerMeterScanner::~PowerMeterScanner(){
    close(fd);
    delete hdrbuf;
}

void PowerMeterScanner::update_reading(){
    std::stringstream ss;
    dev_write("READ?\n");
    reading = std::atof(dev_read());
    timer.start();
}

double PowerMeterScanner::get_reading(){
    return 10e3*reading; //Convert micro watt to milli watt
}
size_t PowerMeterScanner::dev_write(const char *cmd){
    size_t n, len;
    char buf[MAX_CMD_LEN + 16];

    len = strlen(cmd);
    if(len > MAX_CMD_LEN)
    {
        throw std::runtime_error("Device write failed - command too short");
    }

    if(len < 2)
    {
         throw std::runtime_error("Device write failed - command too short");
    }

    strncpy(buf, cmd, MAX_CMD_LEN);
    buf[MAX_CMD_LEN] = 0;

    strcat(buf, "\n");
    n = write(fd, buf, strlen(buf));

    if(n != (len + 1))
    {
        throw std::runtime_error("Size of data sent not equal to data written");
    }

    return len;
}

/*
 * TMC Blockheader ::= #NXXXXXX: is used to describe
 * the length of the data stream, wherein, # is the start denoter of
 * the data stream; N is less than or equal to 9; the N figures
 * followed N represents the length of the data stream in bytes.
 * For example, #9001152054. Wherein, N is 9 and 001152054
 * represents that the data stream contains 1152054 bytes
 * effective data.
 * Reading from the file descriptor blocks,
 * there is a timeout of 5000 milli-Sec.
 */
char* PowerMeterScanner::dev_read(){
    ssize_t size, size2, len;
    char blockhdr[32];

    hdrbuf[0] = 0;
    sz = 0;

    size = read(fd, hdrbuf, MAX_RESP_LEN); //Reads MAX_RESP_LEN bytes from fd to hdrbuf
    if((size < 2) || (size > MAX_RESP_LEN))
    {
        hdrbuf[0] = 0;
        buf[0] = 0;
        throw std::runtime_error("Response size failure during device read:"+std::to_string(size));
    }

    hdrbuf[size] = 0;
    if(hdrbuf[0] != '#')
    {
        if(hdrbuf[size - 1] == '\n')
        {
            hdrbuf[--size] = 0;
        }
        buf = hdrbuf;
        sz = size;
        return buf;
    }

    strncpy(blockhdr, hdrbuf, 16);

    len = blockhdr[1] - '0';

    if((len < 1) || (len > 9))
    {
        blockhdr[31] = 0;
        throw std::runtime_error("blockheader failure during device read");
    }
    blockhdr[len + 2] = 0;
    size2 = atoi(blockhdr + 2); // Convert the string to integer
    size--;  // remove the last character

    if(size < size2)
    {
        blockhdr[31] = 0;
        throw std::runtime_error("blockheader failure during device read");
    }
    buf = hdrbuf + len + 2;
    sz = size2;

    return buf;
}
