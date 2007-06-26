#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <pthread.h>
#include <string>
using namespace std;

typedef string CString;

#define PADR    0x0  /* 00000000 */
#define PACK    0x10 /* 00010000 */
#define PNACK   0x20 /* 00100000 */
#define PSTOP   0x30 /* 00110000 */
#define PCMD    0x40 /* 01000000 */
#define PLEN    0x80 /* 10000000 */
#define PDAT    0xc0 /* 11000000 */

#define CRES    0x1  /* 00000001 Reset command                            */
#define CDIM    0x2  /* 00000010 Dimension data: number of sensors/motors */
#define CDSEN   0x3  /* 00000011 Sensor data values                       */
#define CDMOT   0x4  /* 00000100 Motor data values                        */
#define CBEEP   0x8  /* 00001000 Make a beep    */
#define CDMSG   0x9  /* 00001001 Message data.  */

#define MASK_L2 0xc0 /* 11000000 */
#define MASK_L4 0xf0 /* 11110000 */
#define MASK_R4 0xf  /* 00001111 */

#define MSADR   0x0  /* Master address */

typedef unsigned char uint8;


// pretend to have some windows data types
typedef unsigned long DWORD;
typedef unsigned int UINT;

/** Thread-Klasse, die in eigenem Thread die Kommunikation mit der seriellen Schnittstelle uebernimmt. */
class CSerialThread{
    CString m_port;
    int m_baud;
    bool m_is_running;
    bool terminated;
    bool m_is_joined;
	
    pthread_t thread;
    bool test_mode;
 protected:
    int fd_in; // file handle for incomming
    int fd_out; // file handle for outgoing (=fd_in expect in file test mode)

 public:

    CSerialThread(const CString& port, int baud, bool test_mode=false)
	: m_port(port),m_baud(baud), terminated(false), m_is_joined(true), test_mode(test_mode){};
    virtual ~CSerialThread(){stopandwait();};

    // for communication with inherited class
    
    /// (log) received string 
    virtual void ReceivedCommand() = 0; //const DAT& s) = 0;
    
    // read sensors and write motors
    virtual void writeMotors_readSensors() = 0; //const DAT& s) = 0;

    /// is called in every loop
    virtual void loopCallback() = 0;
    /// is called at the beginning after initialisation of serial
    virtual void Initialise() = 0;

    /// thread is running?
    bool is_running(){return m_is_running;};

    /// start serial communication
    void start();
    /// stop serial communication and wait for the thread to terminate
    void stopandwait();    
    /// stop serial communication (call also be called from inside)
    void stop();    

    /// set com port
    void comport(const CString& port){ m_port=port; };
    /// set baud rate
    void baudrate(int baud){ m_baud=baud; };

    /// thread function
    bool run();
};

#endif
