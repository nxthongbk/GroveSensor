#include <legato.h>
#include <interfaces.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/signal.h>

struct sigaction saio;
struct termios oldtio, newtio;

static char RFID_buffer[64];

static void SerialPortHandler(int fd, short events)
{
    bool is_valid_id = false;
	char read_buffer[64];   /* Buffer to store the data received              */
	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
	int i = 0;
	int start_byte_index = -1;
	int stop_byte_index = -1;

	if (events & POLLIN)    // Data available to read?
    {
		bytes_read = read(fd, &read_buffer, 64);

		if (bytes_read <= 0) {
			LE_ERROR("Read RFID Error");
		}

		if (read_buffer[i] == 0x2) {
			// start byte
			start_byte_index = i;
			if (stop_byte_index != -1) {
				is_valid_id = false;
				start_byte_index = -1;
				stop_byte_index = -1;
			}
		}
		else if (read_buffer[i] == 0x3) {
			// stop byte
			stop_byte_index = i;
			if (start_byte_index >= 0) {
				is_valid_id = true;
			} else {
				is_valid_id = false;
				start_byte_index = -1;
				stop_byte_index = -1;
			}
		}
		if (is_valid_id) {
			memcpy(RFID_buffer,
			       &read_buffer[start_byte_index + 1],
			       stop_byte_index - start_byte_index - 1);
			RFID_buffer[stop_byte_index - start_byte_index] = 0;
		}

		io_PushString("rfid",0,RFID_buffer);
    }
 
    if ((events & POLLERR) || (events & POLLHUP) || (events & POLLRDHUP))   // Error or hang-up?
    {
        LE_ERROR("Failed to get RFID");
    }
}

COMPONENT_INIT
{
	// Open the serial port.
    // Block Signals that we are going to set event handlers for.
    le_sig_Block(SIGPOLL);

	LE_INFO("Start RFID---------------");
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
	tcgetattr (fd, &oldtio);
	tcgetattr (fd, &newtio);

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
	tcsetattr (fd, TCSANOW, &newtio);


	le_fdMonitor_Create("Serial Port",
						fd,				   
						SerialPortHandler,
						POLLIN);		  

	LE_FATAL_IF(io_CreateInput("rfid",
								IO_DATA_TYPE_STRING,
								"string") != LE_OK,
				"FAILED to create output");
}