#ifndef __HSOTPWWRITERPROTOCOL_H__
#define __HSOTPWWRITERPROTOCOL_H__


#define HS_VENDOR_ID					0x0483
#define HS_PRODUCT_ID_OTPWRITER			0x5710

#define HS_6600_VENDOR_ID			    0xF012
#define HS_6600_PRODUCT_ID_OTPWRITER    0x6600

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
f. ͨ��Э��˵����
    i. MEM ��д��
        1) MEM ��д�����еĽ�����ʶ��
            i) �����һ���������ݳ�����Ϊ 0 ����ʶһ�������еĽ�����
        2) MEM ����
            i) ��������һ�� MEM_READ ָ���
            ii) �豸�ظ� MEM_READ ���ݰ�����
                a) ���һ���������ݳ��������Ϊ 0��
            iii) ���豸�޷����ָ��ʱ���ظ�ָ������� CSB �ֽ������� Deny = 1��
        3) MEM д��
            i) �������� MEM_WRITE ָ�������
                a) ÿ�յ�ǰһָ��Ļظ�����ſɷ�����һ��ָ�����
                b) ���һ��ָ��������ݳ��������Ϊ 0��
            ii) �豸ÿ�յ�һ��ָ����ظ�һ�� MEM_WRITE ״̬����
            iii) ���豸�޷����ָ��ʱ���ظ�ָ������� CSB �ֽ������� Deny = 1��
*/
/*
    General Packet format:
        H2D, D2H: <LEN(1), CSB(1), CMDs(1..N), Data(0..(64-1-N-1))>
  
    Note:
        H2D:  Host to Device
        D2H:  Device to Host
        LEN:  Length of the whole packet
        CSB:  Control Status Byte
        CMDs: Command Series, N levels

    1. CMDs ��ı�ʾ��
        ����ָ��ּ�֮���� __ �ָ���һ��ָ���ڲ�������һ�� _ �ָ���
        ���磺HS__MEM__ACCESS_DISABLE����ʾ����ָ�MEM Ϊһ��ָ�ACCESS_DISABLE Ϊ����ָ�

    2. CSB ����״̬�ֽڣ�
        Bit 	����	        ����
        0	    Deny	    =1���ܾ�ָ��
                            =0��ִ��ָ��
        1	    Terminate	=1����ֹ����
                            =0����������
        2..7	Reserved	����


    3. ��λ������λ��ָ�����Ӧ��
        ��. �ܾ�ָ�����ָ����� CSB �ֽ������� Deny = 1��
        ��. ����ָ�
            ��. ִ��ָ���������Ϣ����λ���ȴ���
            ��. ִ����ϣ�����ִ�н����
        ��. ����λ��ָ��Ϊ��ֹ����ָ�����λ������������ֹ��ǰ��������������Ӧ��

    
    4. ��λ������λ������Ӧ��
        ��. ��λ���ܾ�ָ�����ָ��������
        ��. ��λ������ָ�
            ��. ��λ��ִ��ָ��ȴ���λ��������Ϣ��
            ��. ��λ��ִ����ϣ���ʾ�������������ָ�

*/



// Command Level 1
#define HS__MEM				    0x01	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), Command(1), Data(0..61)>
#define HS__CMD				    0x02	// H2D, D2H: <LEN(1), CSB(1), HS__CMD(1), Command(1), Data(0..61)>
#define HS__ERR				    0x03	// H2D, D2H: <LEN(1), CSB(1), HS__ERR(1), ErrCode(1), Data(0..61)>
#define HS__DBG				    0x04	// H2D, D2H: <LEN(1), CSB(1), HS__DBG(1), Command(1), Data(0..61)>

// MEM relevent packet
//// Common operations
#define HS__MEM__ENTER			0x01	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__ENTER(1), Data(0..1)>
#define HS__MEM__EXIT			0x02	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__EXIT(1), Data(0..1)>
#define HS__MEM__READ			0x03	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__READ(1), Address(4), DataLength(4), Data(H2D(0)|D2H(0..52))>
#define HS__MEM__WRITE			0x04	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__WRITE(1), Address(4), DataLength(4), Data(0..52)>

#define HS__MEM__WREN			0x05	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__WREN(1), Data(0..1)>
#define HS__MEM__WRDIS			0x06	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__WRDIS(1), Data(0..1)>
#define HS__MEM__RDSR			0x07	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__RDSR(1), Data(0..1)>
#define HS__MEM__WRSR			0x08	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__WRSR(1), Data(0..1)>
#define HS__MEM__ACCESS_DISABLE	0x09	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__ACCESS_DISABLE(1), Data(0..1)>
#define HS__MEM__READ_DISABLE	0x0a	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__READ_DISABLE(1), Data(0..1)>
#define HS__MEM__WRITE_DISABLE	0x0b	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__WRITE_DISABLE(1), Data(0..1)> // Note: Different from HS__MEM__WRDIS, which is used to reset MEM write enable latch WEN bit in register FSR.
#define HS__MEM__BLKCHK			0x0c	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__BLKCHK(1), Data(0..1)>
//// OTP specific operations
#define HS__MEM__RESET			0x20	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__RESET(1), Data(0..1)>
////// OTP production test
#define HS__MEM__TEST_BLKCHK	0x30	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__TEST_BLKCHK(1), Data(0..1)>
#define HS__MEM__TEST_DEC		0x31	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__TEST_DEC(1), Data(0..1)>
#define HS__MEM__TEST_WR		0x32	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__TEST_WR(1), Data(0..1)>
#define HS__MEM__SPRON			0x33	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__SPRON(1), Data(0..1)>
#define HS__MEM__SPROFF			0x34	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__SPROFF(1), Data(0..1)>

//// Flash specific operations
#define HS__MEM__ERASE_PAGE		0x50	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__ERASE_PAGE(1), PageNumBegin(4), PageNumEnd(4)>
#define HS__MEM__ERASE_ALL		0x51	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__ERASE_ALL(1), Data(H2D(0)|D2H(8))> // return <PageNumBegin(4), PageNumEnd(4)> ereased really
#define HS__MEM__RDFPCR			0x52	// H2D, D2H: <LEN(1), CSB(1), HS__MEM(1), HS__MEM__RDFPCR(1), Data(0..1)]>

// CMD relevent packet
#define HS__CMD__SET__CHIP_TYPE      0x01    // H2D, D2H: <LEN(1), CSB(1), HS__CMD(1), HS__CMD__SET__CHIP_TYPE(1), Data(1)>
#define HS__CMD__GET__CHIP_TYPE      0x02    // H2D, D2H: <LEN(1), CSB(1), HS__CMD(1), HS__CMD__GET__CHIP_TYPE(1), Data(0..1)>
typedef enum _HS_CHIP_TYPE_t {
    HS__CMD__CHIP_TYPE__NONE           =    0x00,
    
    // OTP
    HS__CMD__CHIP_TYPE__OTP__HS6206    =    0x01,
    HS__CMD__CHIP_TYPE__EEPROM_HS66XX  =    0x02,
	HS__CMD__CHIP_TYPE__OTP__HS6210    =    0x03,
    
    // FPGA
    HS__CMD__CHIP_TYPE__FPGA__HS6206   =    0x41,

    // Flash
    HS__CMD__CHIP_TYPE__FLASH__EN25T80 =    0x81,
    HS__CMD__CHIP_TYPE__FLASH_HS66XX   =    0x82
    
} HS_CHIP_TYPE_t;

typedef enum _HS_MEM_TYPE_t {
    MEM_TYPE_NONE           =    0x00,
    MEM_TYPE_FLASH          =    0x01,
    MEM_TYPE_EEPROM         =    0x02,
    MEM_TYPE_OTP            =    0x03
} HS_MEM_TYPE_t;

#define HS__CMD__GET__FIRMWARE_VERSION      0x03    // H2D, D2H: <LEN(1), CSB(1), HS__CMD(1), HS__CMD__GET__FIRMWARE_VERSION(1), Data(0..61)>
#define HS__CMD__GET__CHIP_ID	0x04	// H2D, D2H: <LEN(1), CSB(1), HS__CMD(1), HS__CMD__GET__CHIP_ID(1), CHIP_ID(2, BIT15-BIT0)>


// ERR relevent packet

// DBG relevent packet





#endif