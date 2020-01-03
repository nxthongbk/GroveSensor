#include "legato.h"
#include "interfaces.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/signal.h>

static bool data_available = false;
static int cmd_index = -1;
void signal_handler_IO(int status);

struct sigaction saio;
struct termios oldtio, newtio;
static char speech_buffer[64];

const char *voiceBuffer[] =
{
    "Turn on the light",
    "Turn off the light",
    "Play music",
    "Pause",
    "Next",
    "Previous",
    "Up",
    "Down",
    "Turn on the TV",
    "Turn off the TV",
    "Increase temperature",
    "Decrease temperature",
    "What's the time",
    "Open the door",
    "Close the door",
    "Left",
    "Right",
    "Stop",
    "Start",
    "Mode 1",
    "Mode 2",
    "Go",
};

static void SerialPortHandler(int fd, short events)
{
    char read_buffer[64]; /* Buffer to store the data received              */
    int bytes_read = 0;   /* Number of bytes read by the read() system call */
    int i = 0;

    if (events & POLLIN) // Data available to read?
    {
        bytes_read = read(fd, &read_buffer, 10);
        if (bytes_read <= 0)
        {
            data_available = false;
        }
        for (i = 0; i < bytes_read; i++)
        {
            if (read_buffer[i] >= 1 && read_buffer[i] <= sizeof(voiceBuffer))
            {
                cmd_index = read_buffer[i] - 1;
            }
            else
            {
                LE_ERROR("Unknown Command '%d'\n", read_buffer[i]);
            }
        }

       	memcpy(speech_buffer, voiceBuffer[cmd_index], strlen(voiceBuffer[cmd_index]));
       	speech_buffer[strlen(voiceBuffer[cmd_index])] = 0;

        LE_INFO(speech_buffer);
        io_PushString("speech_recognizer", 0, speech_buffer);
    }

    if ((events & POLLERR) || (events & POLLHUP) || (events & POLLRDHUP)) // Error or hang-up?
    {
        LE_ERROR("Failed to get speech recoginer");
    }
}

void speechRecognizer_Read(char* cmd, size_t id_bufferSize)
{
    if (id_bufferSize <= strlen(speech_buffer))
    {
        LE_ERROR("Input buffer did not have enough size");
    }

    memcpy(cmd,
           speech_buffer,
           strlen(speech_buffer));
    cmd[strlen(speech_buffer)] = 0;
}

COMPONENT_INIT
{
    // Open the serial port.
    // Block Signals that we are going to set event handlers for.
    le_sig_Block(SIGPOLL);

    int fd = open("/dev/ttyHS0", O_RDWR|O_NONBLOCK);
    LE_FATAL_IF(fd == -1, "open failed with errno %d (%m)", errno);

    // saio.sa_mask = 0;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO, &saio, NULL);

    /* allow the process to receive SIGIO */
    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, FASYNC);

    // get the parameters
    tcgetattr(fd, &oldtio);
    tcgetattr(fd, &newtio);

    // Set the baud rates to 9600...
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    // Enable the receiver and set local mode...
    newtio.c_cflag |= (CLOCAL | CREAD);
    // No parity (8N1):
    newtio.c_cflag &= ~PARENB;
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    // enable hardware flow control (CNEW_RTCCTS)
    // newtio.c_cflag |= CRTSCTS;
    // if(hw_handshake)
    // Disable the hardware flow control for use with mangOH
    newtio.c_cflag &= ~CRTSCTS;

    // set raw input
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    newtio.c_iflag &= ~(INLCR | ICRNL | IGNCR);

    // set raw output
    newtio.c_oflag &= ~OPOST;
    newtio.c_oflag &= ~OLCUC;
    newtio.c_oflag &= ~ONLRET;
    newtio.c_oflag &= ~ONOCR;
    newtio.c_oflag &= ~OCRNL;

    newtio.c_cc[VMIN] = 0;
    newtio.c_cc[VTIME] = 0;

    // Set the new newtio for the port...
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    le_fdMonitor_Create("Serial Port",
                        fd,
                        SerialPortHandler,
                        POLLIN);

    LE_FATAL_IF(io_CreateInput("speech_recognizer",
                               IO_DATA_TYPE_STRING,
                               "string") != LE_OK,
                "FAILED to create input speech_recoginer");
}
