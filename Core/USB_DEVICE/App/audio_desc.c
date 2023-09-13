#include "audio_desc.h"

#include "usbd_audio.h"

const uint8_t USBD_AUDIO_CfgDesc[USB_AUDIO_CONFIG_DESC_SIZE] __attribute__ ((aligned (4))) =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  LOBYTE(USB_AUDIO_CONFIG_DESC_SIZE),    /* wTotalLength */
  HIBYTE(USB_AUDIO_CONFIG_DESC_SIZE),
  0x02,                                 /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
  0xC0,                                 /* bmAttributes: Bus Powered according to user configuration */
#else
  0x80,                                 /* bmAttributes: Bus Powered according to user configuration */
#endif /* USBD_SELF_POWERED */
  USBD_MAX_POWER,                       /* MaxPower (mA) */

	// IAD: See Audio20 section 4.6
	0x08,								// bLength
	USB_DESC_TYPE_IAD,	// bDescriptorType
	AC_INTERFACE_NUM,								// bFirstInterface
	0x02,								// bInterfaceCount
	AUDIO_FUNCTION,			// bFunctionClass
	FUNCTION_SUBCLASS_UNDEFINED,								// bFunctionSubClass
	AF_VERSION_02_00,								// bFunctionProtocol
	0x00,								// iFunction

	// AC interface descriptor: See Audio20 section 4.7
	// Standard
	0x09,											// bLength
	USB_DESC_TYPE_INTERFACE,	// bDescriptorType
	AC_INTERFACE_NUM,											// bInterfaceNumber
	0x00,											// bAlternateSetting
	0x00,											// bNumEndpoints
	AUDIO, 										// bInterfaceClass
	AUDIOCONTROL,							// bInterfaceSubClass
	IP_VERSION_02_00,					// bInterfaceProtocol
	0x00,											// iInterface

	// Class specific
	0x09,													// bLength
	CS_INTERFACE,									// bDescriptorType
	HEADER,												// bDescriptorSubtype
	0x00,													// bcdADC
	0x02,
	FUNCTION_SUBCLASS_UNDEFINED,	// bCategory
	LOBYTE(AUDIO_WTOTALLENGTH),		// wTotalLength
	HIBYTE(AUDIO_WTOTALLENGTH),
	0x00,													// bmControls

	// Clock source
	0x08,													// bLength
	CS_INTERFACE,									// bDescriptorType
	CLOCK_SOURCE,									// bDescriptorSubtype
	CLOCK_SOURCE_ID,							// bClockID
	0x03,													// bmAttributes
	0x03,													// bmControls: see Audio20 4.7.2.1
	0x00,													// bAssocTerminal
	0x00,													// iClockSource

	// Input terminal
	17U,													// bLength
	CS_INTERFACE,									// bDescriptorType
	INPUT_TERMINAL,								// bDescriptorSubtype
	INPUT_TERMINAL_ID,													// bTerminalID
	LOBYTE(INPUT_TERMINAL_TYPE),	// wTerminalType
	HIBYTE(INPUT_TERMINAL_TYPE),
	0x00,													// bAssocTerminal
	CLOCK_SOURCE_ID,							// bCSourceID
	0x02,													// bNrChannels
	0x03,													// bmChannelConfig
	0x00,
	0x00,
	0x00,
	0x00, 												// iChannelNames
	0x00, 												// bmControls
	0x00,
	0x00,													// iTerminal

	// Feature unit
	14U,													// bLength
	CS_INTERFACE,									// bDescriptorType
	FEATURE_UNIT,									// bDescriptorSubtype
	FEATURE_UNIT_ID,							// bUnitID
	INPUT_TERMINAL_ID,						// bSourceID
	0x0f,													// bmaControls(0)
	0x00,
	0x00,
	0x00,
	0x0f,													// bmaControls(1)
	0x00,
	0x00,
	0x00,
	0x00,													// iFeature

	// Output terminal
	12U,													// bLength
	CS_INTERFACE,									// bDescriptorType
	OUTPUT_TERMINAL,							// bDescriptorSubtype
	OUTPUT_TERMINAL_ID,						// bTerminalID
	LOBYTE(OUTPUT_TERMINAL_TYPE),	// wTerminalType
	HIBYTE(OUTPUT_TERMINAL_TYPE),
	0x00,													// bAssocTerminal
	FEATURE_UNIT_ID,							// bSourceID
	CLOCK_SOURCE_ID,							// bCSourceID
	0x00,													// bmControls
	0x00,
	0x00,													// iTerminal

	// AS interface descriptor: see Audio20 section 4.9
	// Standard
	// Alternate settings: see https://learn.microsoft.com/en-us/windows-hardware/drivers/audio/usb-2-0-audio-drivers
	// Alternate setting 0
	0x09,													// bLength
	USB_DESC_TYPE_INTERFACE,			// bDescriptorType
	AS_INTERFACE_NUM,							// bInterfaceNumber
	0x00, 												// bAlternateSetting
	0x00,													// bNumEndpoints
	AUDIO,												// bInterfaceClass
	AUDIOSTREAMING,								// bInterfaceSubClass
	IP_VERSION_02_00,							// bInterfaceProtocol
	0x00,													// iInterface

	// Alternate setting 1
	0x09,													// bLength
	USB_DESC_TYPE_INTERFACE,			// bDescriptorType
	AS_INTERFACE_NUM,							// bInterfaceNumber
	0x01, 												// bAlternateSetting
	0x02,													// bNumEndpoints
	AUDIO,												// bInterfaceClass
	AUDIOSTREAMING,								// bInterfaceSubClass
	IP_VERSION_02_00,							// bInterfaceProtocol
	0x00,													// iInterface

	// Class specific
	16U,													// bLength
	CS_INTERFACE,									// bDescriptorType
	AS_GENERAL,										// bDescriptorSubtype
	INPUT_TERMINAL_ID,						// bTerminalLink
	0x00,													// bmControls
	FORMAT_TYPE_I,								// bFormatType
	0x01,													// bmFormats: PCM; Check Frmts20 section A.2
	0x00,
	0x00,
	0x00,
	0x02,													// bNrChannels
	0x03,													// bmChannelConfig: FL, FR; See Audio20 section 4.1
	0x00,
	0x00,
	0x00,
	0x00,													// iChannelNames

	// Format type I descriptor
	0x06,													// bLength
	CS_INTERFACE,									// bDescriptorType
	FORMAT_TYPE,									// bDescriptorSubtype
	FORMAT_TYPE_I,								// bFormatType
	0x04,													// bSubslotSize
	32U,													// bBitResolution

	// AS audio data endpoint descriptor: see Audio20 section 4.10
	// Standard
	0x07,													// bLength
	USB_DESC_TYPE_ENDPOINT,				// bDescriptorType
	STREAMING_EP_ADDR,						// bEndpointAddress
	STREAMING_EP_ATTRIB,					// bmAttributes
	LOBYTE(USB_HS_MAX_PACKET_SIZE),				// wMaxPacketSize
	HIBYTE(USB_HS_MAX_PACKET_SIZE),
	STREAMING_HS_BINTERVAL,						// bInterval

	// Class specific
	0x08,													// bLength
	CS_ENDPOINT,									// bDescriptorType
	EP_GENERAL,										// bDescriptorSubtype
	0x00,													// bmAttributes
	0x00,													// bmControls
	0x00,													// bLockDelayUnits
	0x00,													// wLockDelay
	0x00,

	// AS audio feedback endpoint descriptor
	// Standard
	0x07, 												// bLength
	USB_DESC_TYPE_ENDPOINT,				// bDescriptorType
	FEEDBACK_EP_ADDR,							// bEndpointAddress
	FEEDBACK_EP_ATTRIB,						// bmAttributes
	LOBYTE(FEEDBACK_PACKET_SIZE),	// wMaxPacketSize
	HIBYTE(FEEDBACK_PACKET_SIZE),
	FEEDBACK_HS_BINTERVAL,					// bInterval

	// Alternate setting 2
	0x09,													// bLength
	USB_DESC_TYPE_INTERFACE,			// bDescriptorType
	AS_INTERFACE_NUM,							// bInterfaceNumber
	0x02, 												// bAlternateSetting
	0x02,													// bNumEndpoints
	AUDIO,												// bInterfaceClass
	AUDIOSTREAMING,								// bInterfaceSubClass
	IP_VERSION_02_00,							// bInterfaceProtocol
	0x00,													// iInterface

	// Class specific
	16U,													// bLength
	CS_INTERFACE,									// bDescriptorType
	AS_GENERAL,										// bDescriptorSubtype
	INPUT_TERMINAL_ID,						// bTerminalLink
	0x00,													// bmControls
	FORMAT_TYPE_I,								// bFormatType
	0x01,													// bmFormats: PCM; Check Frmts20 section A.2
	0x00,
	0x00,
	0x00,
	0x02,													// bNrChannels
	0x03,													// bmChannelConfig: FL, FR; See Audio20 section 4.1
	0x00,
	0x00,
	0x00,
	0x00,													// iChannelNames

	// Format type I descriptor
	0x06,													// bLength
	CS_INTERFACE,									// bDescriptorType
	FORMAT_TYPE,									// bDescriptorSubtype
	FORMAT_TYPE_I,								// bFormatType
	0x04,													// bSubslotSize
	24U,													// bBitResolution

	// AS audio data endpoint descriptor: see Audio20 section 4.10
	// Standard
	0x07,													// bLength
	USB_DESC_TYPE_ENDPOINT,				// bDescriptorType
	STREAMING_EP_ADDR,						// bEndpointAddress
	STREAMING_EP_ATTRIB,					// bmAttributes
	LOBYTE(USB_HS_MAX_PACKET_SIZE),				// wMaxPacketSize
	HIBYTE(USB_HS_MAX_PACKET_SIZE),
	STREAMING_HS_BINTERVAL,						// bInterval

	// Class specific
	0x08,													// bLength
	CS_ENDPOINT,									// bDescriptorType
	EP_GENERAL,										// bDescriptorSubtype
	0x00,													// bmAttributes
	0x00,													// bmControls
	0x00,													// bLockDelayUnits
	0x00,													// wLockDelay
	0x00,

	// AS audio feedback endpoint descriptor
	// Standard
	0x07, 												// bLength
	USB_DESC_TYPE_ENDPOINT,				// bDescriptorType
	FEEDBACK_EP_ADDR,							// bEndpointAddress
	FEEDBACK_EP_ATTRIB,						// bmAttributes
	LOBYTE(FEEDBACK_PACKET_SIZE),	// wMaxPacketSize
	HIBYTE(FEEDBACK_PACKET_SIZE),
	FEEDBACK_HS_BINTERVAL,					// bInterval
};

/* USB Standard Device Descriptor */
const uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __attribute__ ((aligned (4))) =
{
  USB_LEN_DEV_QUALIFIER_DESC,			// bLength
  USB_DESC_TYPE_DEVICE_QUALIFIER,	// bDescriptorType
  0x00,														// bcdUSB
	0x02,

	// See Audio20 section 4.3
  0xef,														// bDeviceClass
  0x02,														// bDeviceSubClass
  0x01,														// bDeviceProtocol

  USB_MAX_EP0_SIZE,								// bMaxPacketSize0
  0x00,
  0x00,
};
