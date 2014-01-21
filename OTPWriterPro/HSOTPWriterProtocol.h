#ifndef __HSOTPWWRITERPROTOCOL_H__
#define __HSOTPWWRITERPROTOCOL_H__


#define HS_VENDOR_ID					0x0483
#define HS_PRODUCT_ID_OTPWRITER			0x5710

#define HS_OTPWRITER_HID_REPORT_SIZE	64
#define HS_OTPWRITER_HID_USAGE_PAGE		0xFF99
#define HS_OTPWRITER_HID_USAGE_PAGEH	0xFF
#define HS_OTPWRITER_HID_USAGE_PAGEL	0x99
#define HS_OTPWRITER_HID_USAGE_0		0
#define HS_OTPWRITER_HID_USAGE_1		1
#define HS_OTPWRITER_HID_USAGE_63		63

const unsigned char OTPWriter_ReportDescriptor[] =
{
	0x06, HS_OTPWRITER_HID_USAGE_PAGEL,	HS_OTPWRITER_HID_USAGE_PAGEH,	// Usage Page (HS_OTPWRITER_HID_USAGE_PAGE) 
	0x09, HS_OTPWRITER_HID_USAGE_1,										// Usage (HS_OTPWRITER_HID_USAGE_1) 
	0xA1, 0x01,															// Collection (Application) 

	0x75, 0x08,																// Report Size (8) 
	0x95, HS_OTPWRITER_HID_REPORT_SIZE,										// Report Count (HS_OTPWRITER_HID_REPORT_SIZE) 
	0x15, 0x00,																// Logical Minimum (0) 
	0x25, 0xFF,  															// Logical Maximum (255) 
																			   
	0x19, HS_OTPWRITER_HID_USAGE_0,											// Usage Minimum (HS_OTPWRITER_HID_USAGE_0) 
	0x29, HS_OTPWRITER_HID_USAGE_63,										// Usage Maximum (HS_OTPWRITER_HID_USAGE_63) 
	0x81, 0x02,																// Input (Data,Var,Abs) 
																			   
	0x19, HS_OTPWRITER_HID_USAGE_0,											// Usage Minimum (HS_OTPWRITER_HID_USAGE_0) 
	0x29, HS_OTPWRITER_HID_USAGE_63,										// Usage Maximum (HS_OTPWRITER_HID_USAGE_63) 
	0x91, 0x02,																// Output (Data,Var,Abs) 

	0xC0																// End Collection 

}; /* OTPWriter_ReportDescriptor */

/*
f. 通信协议说明：
	i. MEM 读写：
		1) MEM 读写包序列的结束标识：
			i) 以最后一个包的数据长度域为 0 来标识一个包序列的结束。
		2) MEM 读：
			i) 主机发送一个 MEM_READ 指令包
			ii) 设备回复 MEM_READ 数据包序列
				a) 最后一个包的数据长度域必须为 0。
			iii) 当设备无法完成指令时，回复 PID_ERR 包。
		3) MEM 写：
			i) 主机发送 MEM_WRITE 指令包序列
				a) 每收到前一指令的回复包后才可发送下一个指令包；
				b) 最后一个指令包的数据长度域必须为 0。
			ii) 设备每收到一个指令包回复一个 MEM_WRITE 状态包。
			iii) 当设备无法完成指令时，回复 PID_ERR 包。


*/

// Packet ID
#define HS_PID_MEM				0x01	// H2D, D2H: <PL(1), HS_PID_MEM(1), Command(1), Data(0..61)>
#define HS_PID_CMD				0x02	// H2D, D2H: <PL(1), HS_PID_CMD(1), Command(1), Data(0..61)>
#define HS_PID_ERR				0x03	// H2D, D2H: <PL(1), HS_PID_ERR(1), ErrCode(1), Data(0..61)>
#define HS_PID_DBG				0x04	// H2D, D2H: <PL(1), HS_PID_DBG(1), Command(1), Data(0..61)>

// MEM relevent packet
//// Common operations
#define HS_MEM_ENTER			0x01	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_ENTER(1)>
#define HS_MEM_EXIT				0x02	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_EXIT(1)>
#define HS_MEM_READ				0x03	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_READ(1), Address(4), DataLength(4), Data(0..53)>
#define HS_MEM_WRITE			0x04	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_WRITE(1), Address(4), DataLength(4), Data(0..53)>

#define HS_MEM_WREN				0x05	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_WREN(1)>
#define HS_MEM_WRDIS			0x06	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_WRDIS(1)>
#define HS_MEM_RDSR				0x07	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_RDSR(1), Data(0..1)>
#define HS_MEM_WRSR				0x08	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_WRSR(1), Data(0..1)>
#define HS_MEM_ACCESS_DISABLE	0x09	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_ACCESS_DISABLE(1)>
#define HS_MEM_READ_DISABLE		0x0a	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_READ_DISABLE(1)>
#define HS_MEM_WRITE_DISABLE	0x0b	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_WRITE_DISABLE(1)> // Note: Different from HS_MEM_WRDIS, which is used to reset MEM write enable latch WEN bit in register FSR.
#define HS_MEM_BLKCHK			0x0c	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_BLKCHK(1)>
//// OTP specific operations
#define HS_MEM_RESET			0x20	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_RESET(1)>
////// OTP production test
#define HS_MEM_TEST_BLKCHK		0x30	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_TEST_BLKCHK(1)>
#define HS_MEM_TEST_DEC			0x31	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_TEST_DEC(1)>
#define HS_MEM_TEST_WR			0x32	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_TEST_WR(1)>
#define HS_MEM_SPRON			0x33	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_SPRON(1)>
#define HS_MEM_SPROFF			0x34	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_SPROFF(1)>

//// Flash specific operations
#define HS_MEM_ERASE_PAGE		0x50	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_ERASE_PAGE(1), PageNumBegin(4), PageNumEnd(4)>
#define HS_MEM_ERASE_ALL		0x51	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_ERASE_ALL(1)>
#define HS_MEM_RDFPCR			0x52	// H2D, D2H: <PL(1), HS_PID_MEM(1), HS_MEM_RDFPCR(1), Data(0..1)]>

// CMD relevent packet

// ERR relevent packet

// DBG relevent packet





#endif