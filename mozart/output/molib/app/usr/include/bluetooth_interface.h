/**
 * @file bluetooth_interface.h
 * @brief For the operation of the Bluetooth API
 * @author <zhe.wu@ingenic.com>
 * @version 1.0.0
 * @date 2015-06-15
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co., Ltd.
 *
 * The program is not free, Ingenic without permission,
 * no one shall be arbitrarily (including but not limited
 * to: copy, to the illegal way of communication, display,
 * mirror, upload, download) use, or by unconventional
 * methods (such as: malicious intervention Ingenic data)
 * Ingenic's normal service, no one shall be arbitrarily by
 * software the program automatically get Ingenic data
 * Otherwise, Ingenic will be investigated for legal responsibility
 * according to law.
 */

#ifndef __BLUETOOTH_INTERFACE_H_
#define __BLUETOOTH_INTERFACE_H_

#undef TRUE
#define BSA_DISC_VID_PID_MAX		1
#define HCI_EXT_INQ_RESPONSE_LEN        240
#define BD_ADDR_LEN			6
#define BD_NAME_LEN			248
#define DEV_CLASS_LEN			3
#define BSA_EIR_DATA_LENGTH		HCI_EXT_INQ_RESPONSE_LEN
#define BLE_INCLUDED			TRUE
#define DEVICE_FROM_DATABSASE 		0
#define DEVICE_FROM_DISCOVERY		1

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;
typedef unsigned char   BOOLEAN;

typedef UINT8		tBTA_HH_PROTO_MODE;
typedef UINT8		tBT_DEVICE_TYPE;
typedef UINT8		BD_ADDR[BD_ADDR_LEN];
typedef UINT8		BD_NAME[BD_NAME_LEN + 1];	/* Device name */
typedef UINT8		DEV_CLASS[DEV_CLASS_LEN];	/* Device class */
typedef UINT16		tBSA_STATUS;
typedef UINT32		tBTA_SERVICE_MASK;
typedef tBTA_SERVICE_MASK   	tBSA_SERVICE_MASK;
typedef UINT16			tGATT_PERM;

typedef struct {
	UINT8 data_length;
	UINT16 company_id;
	UINT8 *p_manu;
} bsa_manu_data;

#define LINK_KEY_LEN    16
typedef UINT8 LINK_KEY[LINK_KEY_LEN];       /* Link Key */

#define BT_OCTET8_LEN    8
typedef UINT8 BT_OCTET8[BT_OCTET8_LEN];   /* octet array: size 16 */

/*****************************************************************************
 **  BLE Constants and Type Definitions
 *****************************************************************************/
#ifndef BSA_BLE_DEBUG
#define BSA_BLE_DEBUG FALSE
#endif

/* ADV data flag bit definition used for BTM_BLE_AD_TYPE_FLAG */
#define BTM_BLE_LIMIT_DISC_FLAG         (0x01 << 0)             /* bit 0 */
#define BTM_BLE_GEN_DISC_FLAG           (0x01 << 1)             /* bit 1 */
#define BTM_BLE_BREDR_NOT_SPT           (0x01 << 2)             /* bit 2*/
/* 4.1 spec adv flag for simultaneous BR/EDR+LE connection support */
#define BTM_BLE_DMT_CONTROLLER_SPT      (0x01 << 3)             /* bit 3 */
#define BTM_BLE_DMT_HOST_SPT            (0x01 << 4)             /* bit 4 */
#define BTM_BLE_NON_LIMIT_DISC_FLAG     (0x00 )         /* lowest bit unset */
#define BTM_BLE_ADV_FLAG_MASK           (BTM_BLE_LIMIT_DISC_FLAG | BTM_BLE_BREDR_NOT_SPT | BTM_BLE_GEN_DISC_FLAG)
#define BTM_BLE_LIMIT_DISC_MASK         (BTM_BLE_LIMIT_DISC_FLAG )

#define BTM_BLE_AD_BIT_DEV_NAME        (0x00000001 << 0)
#define BTM_BLE_AD_BIT_FLAGS           (0x00000001 << 1)
#define BTM_BLE_AD_BIT_MANU            (0x00000001 << 2)
#define BTM_BLE_AD_BIT_TX_PWR          (0x00000001 << 3)
#define BTM_BLE_AD_BIT_INT_RANGE       (0x00000001 << 5)
#define BTM_BLE_AD_BIT_SERVICE         (0x00000001 << 6)
#define BTM_BLE_AD_BIT_SERVICE_SOL     (0x00000001 << 7)
#define BTM_BLE_AD_BIT_SERVICE_DATA    (0x00000001 << 8)
#define BTM_BLE_AD_BIT_SIGN_DATA       (0x00000001 << 9)
#define BTM_BLE_AD_BIT_SERVICE_128SOL  (0x00000001 << 10)
#define BTM_BLE_AD_BIT_APPEARANCE      (0x00000001 << 11)
#define BTM_BLE_AD_BIT_PUBLIC_ADDR     (0x00000001 << 12)
#define BTM_BLE_AD_BIT_RANDOM_ADDR     (0x00000001 << 13)
#define BTM_BLE_AD_BIT_SERVICE_32      (0x00000001 << 4)
#define BTM_BLE_AD_BIT_SERVICE_32SOL   (0x00000001 << 14)
#define BTM_BLE_AD_BIT_PROPRIETARY     (0x00000001 << 15)
#define BTM_BLE_AD_BIT_SERVICE_128     (0x00000001 << 16)      /*128-bit Service UUIDs*/

/* type of protocol mode */
#define BTA_HH_PROTO_RPT_MODE                   (0x00)
#define BTA_HH_PROTO_BOOT_MODE                  (0x01)
#define BTA_HH_PROTO_UNKNOWN                    (0xff)
/* ADV data flag bit definition used for BTM_BLE_AD_TYPE_FLAG */
#define BSA_DM_BLE_LIMIT_DISC_FLAG         BTM_BLE_LIMIT_DISC_FLAG
#define BSA_DM_BLE_GEN_DISC_FLAG           BTM_BLE_GEN_DISC_FLAG
#define BSA_DM_BLE_BREDR_NOT_SPT           BTM_BLE_BREDR_NOT_SPT
#define BSA_DM_BLE_NON_LIMIT_DISC_FLAG     BTM_BLE_NON_LIMIT_DISC_FLAG         /* lowest bit unset */
#define BSA_DM_BLE_ADV_FLAG_MASK           BTM_BLE_ADV_FLAG_MASK
#define BSA_DM_BLE_LIMIT_DISC_MASK         BTM_BLE_LIMIT_DISC_MASK

#define BSA_DM_BLE_AD_DATA_LEN          31   /*BLE Advertisement data size limit, stack takes 31bytes of data */
#define BSA_DM_BLE_AD_UUID_MAX          6   /*Max number of Service UUID the device can advertise*/
#define APP_DISC_NB_DEVICES		20
#define BSA_BLE_CL_WRITE_MAX		100
#define GATT_MAX_ATTR_LEN		600
#define APP_BLE_MAIN_DEFAULT_APPL_UUID	9000
#define BLE_ADV_APPEARANCE_DATA         0x832	/* Generic Heart rate Sensor */
#define APP_BLE_ADV_VALUE_LEN           13  /*This is temporary value, Total Adv data including all fields should be <31bytes*/
#define BTA_GATTS_INVALID_APP   	0xff
#define BTA_GATTS_INVALID_IF    	0
#define BSA_BLE_MAX_ATTR_LEN  		  GATT_MAX_ATTR_LEN
#define BSA_DM_BLE_ADV_FLAG_MASK          BTM_BLE_ADV_FLAG_MASK
#define BSA_DM_BLE_AD_BIT_DEV_NAME        BTM_BLE_AD_BIT_DEV_NAME
#define BSA_DM_BLE_AD_BIT_FLAGS           BTM_BLE_AD_BIT_FLAGS
#define BSA_DM_BLE_AD_BIT_MANU            BTM_BLE_AD_BIT_MANU
#define BSA_DM_BLE_AD_BIT_TX_PWR          BTM_BLE_AD_BIT_TX_PWR
#define BSA_DM_BLE_AD_BIT_INT_RANGE       BTM_BLE_AD_BIT_INT_RANGE
#define BSA_DM_BLE_AD_BIT_SERVICE         BTM_BLE_AD_BIT_SERVICE
#define BSA_DM_BLE_AD_BIT_SERVICE_SOL     BTM_BLE_AD_BIT_SERVICE_SOL
#define BSA_DM_BLE_AD_BIT_SERVICE_DATA    BTM_BLE_AD_BIT_SERVICE_DATA
#define BSA_DM_BLE_AD_BIT_SIGN_DATA       BTM_BLE_AD_BIT_SIGN_DATA
#define BSA_DM_BLE_AD_BIT_SERVICE_128     BTM_BLE_AD_BIT_SERVICE_128
#define BSA_DM_BLE_AD_BIT_SERVICE_128SOL  BTM_BLE_AD_BIT_SERVICE_128SOL
#define BSA_DM_BLE_AD_BIT_APPEARANCE      BTM_BLE_AD_BIT_APPEARANCE
#define BSA_DM_BLE_AD_BIT_PUBLIC_ADDR     BTM_BLE_AD_BIT_PUBLIC_ADDR
#define BSA_DM_BLE_AD_BIT_RANDOM_ADDR     BTM_BLE_AD_BIT_RANDOM_ADDR
#define BSA_DM_BLE_AD_BIT_PROPRIETARY     BTM_BLE_AD_BIT_PROPRIETARY

#define BT_EIR_FLAGS_TYPE                   0x01
#define BT_EIR_MORE_16BITS_UUID_TYPE        0x02
#define BT_EIR_COMPLETE_16BITS_UUID_TYPE    0x03
#define BT_EIR_MORE_32BITS_UUID_TYPE        0x04
#define BT_EIR_COMPLETE_32BITS_UUID_TYPE    0x05
#define BT_EIR_MORE_128BITS_UUID_TYPE       0x06
#define BT_EIR_COMPLETE_128BITS_UUID_TYPE   0x07
#define BT_EIR_SHORTENED_LOCAL_NAME_TYPE    0x08
#define BT_EIR_COMPLETE_LOCAL_NAME_TYPE     0x09
#define BT_EIR_TX_POWER_LEVEL_TYPE          0x0A
#define BT_EIR_OOB_BD_ADDR_TYPE             0x0C
#define BT_EIR_OOB_COD_TYPE                 0x0D
#define BT_EIR_OOB_SSP_HASH_C_TYPE          0x0E
#define BT_EIR_OOB_SSP_RAND_R_TYPE          0x0F
#define BT_EIR_OOB_SSP_HASH_C_256_TYPE      0x1D
#define BT_EIR_OOB_SSP_RAND_R_256_TYPE      0x1E
#define BT_EIR_3D_SYNC_TYPE                 0x3D
#define BT_EIR_MANUFACTURER_SPECIFIC_TYPE   0xFF

/* Definitions for Extended Inquiry Response */
#define HCI_EXT_INQ_RESPONSE_LEN        240
#define HCI_EIR_FLAGS_TYPE                   BT_EIR_FLAGS_TYPE
#define HCI_EIR_MORE_16BITS_UUID_TYPE        BT_EIR_MORE_16BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_16BITS_UUID_TYPE    BT_EIR_COMPLETE_16BITS_UUID_TYPE
#define HCI_EIR_MORE_32BITS_UUID_TYPE        BT_EIR_MORE_32BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_32BITS_UUID_TYPE    BT_EIR_COMPLETE_32BITS_UUID_TYPE
#define HCI_EIR_MORE_128BITS_UUID_TYPE       BT_EIR_MORE_128BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_128BITS_UUID_TYPE   BT_EIR_COMPLETE_128BITS_UUID_TYPE
#define HCI_EIR_SHORTENED_LOCAL_NAME_TYPE    BT_EIR_SHORTENED_LOCAL_NAME_TYPE
#define HCI_EIR_COMPLETE_LOCAL_NAME_TYPE     BT_EIR_COMPLETE_LOCAL_NAME_TYPE
#define HCI_EIR_TX_POWER_LEVEL_TYPE          BT_EIR_TX_POWER_LEVEL_TYPE
#define HCI_EIR_OOB_BD_ADDR_TYPE             BT_EIR_OOB_BD_ADDR_TYPE
#define HCI_EIR_OOB_COD_TYPE                 BT_EIR_OOB_COD_TYPE
#define HCI_EIR_OOB_SSP_HASH_C_TYPE          BT_EIR_OOB_SSP_HASH_C_TYPE
#define HCI_EIR_OOB_SSP_RAND_R_TYPE          BT_EIR_OOB_SSP_RAND_R_TYPE
#define HCI_EIR_OOB_SSP_HASH_C_256_TYPE      BT_EIR_OOB_SSP_HASH_C_256_TYPE
#define HCI_EIR_OOB_SSP_RAND_R_256_TYPE      BT_EIR_OOB_SSP_RAND_R_256_TYPE
#define HCI_EIR_3D_SYNC_TYPE                 BT_EIR_3D_SYNC_TYPE
#define HCI_EIR_MANUFACTURER_SPECIFIC_TYPE   BT_EIR_MANUFACTURER_SPECIFIC_TYPE


#define BTM_BLE_AD_TYPE_FLAG            HCI_EIR_FLAGS_TYPE                  /* 0x01 */
#define BTM_BLE_AD_TYPE_16SRV_PART      HCI_EIR_MORE_16BITS_UUID_TYPE       /* 0x02 */
#define BTM_BLE_AD_TYPE_16SRV_CMPL      HCI_EIR_COMPLETE_16BITS_UUID_TYPE   /* 0x03 */
#define BTM_BLE_AD_TYPE_32SRV_PART      HCI_EIR_MORE_32BITS_UUID_TYPE       /* 0x04 */
#define BTM_BLE_AD_TYPE_32SRV_CMPL      HCI_EIR_COMPLETE_32BITS_UUID_TYPE   /* 0x05 */
#define BTM_BLE_AD_TYPE_128SRV_PART     HCI_EIR_MORE_128BITS_UUID_TYPE       /* 0x06 */
#define BTM_BLE_AD_TYPE_128SRV_CMPL     HCI_EIR_COMPLETE_128BITS_UUID_TYPE   /* 0x07 */
#define BTM_BLE_AD_TYPE_NAME_SHORT      HCI_EIR_SHORTENED_LOCAL_NAME_TYPE       /* 0x08 */
#define BTM_BLE_AD_TYPE_NAME_CMPL       HCI_EIR_COMPLETE_LOCAL_NAME_TYPE        /* 0x09 */
#define BTM_BLE_AD_TYPE_TX_PWR          HCI_EIR_TX_POWER_LEVEL_TYPE             /* 0x0A */
#define BTM_BLE_AD_TYPE_DEV_CLASS       0x0D
#define BTM_BLE_AD_TYPE_SM_TK           0x10
#define BTM_BLE_AD_TYPE_SM_OOB_FLAG     0x11
#define BTM_BLE_AD_TYPE_INT_RANGE       0x12
#define BTM_BLE_AD_TYPE_SOL_SRV_UUID    0x14
#define BTM_BLE_AD_TYPE_128SOL_SRV_UUID 0x15
#define BTM_BLE_AD_TYPE_SERVICE_DATA    0x16
#define BTM_BLE_AD_TYPE_PUBLIC_TARGET   0x17
#define BTM_BLE_AD_TYPE_RANDOM_TARGET   0x18
#define BTM_BLE_AD_TYPE_APPEARANCE      0x19
#define BTM_BLE_AD_TYPE_ADV_INT         0x1a
#define BTM_BLE_AD_TYPE_32SOL_SRV_UUID    0x1b  /* TBD */
#define BTM_BLE_AD_TYPE_32SERVICE_DATA    0x1c  /* TBD */
#define BTM_BLE_AD_TYPE_128SERVICE_DATA    0x1d /* TBD */

/* AV callback events */
#define BSA_AVK_OPEN_EVT         2       /* connection opened */
#define BSA_AVK_CLOSE_EVT        3       /* connection closed */
#define BSA_AVK_START_EVT        4       /* stream data transfer started */
#define BSA_AVK_STOP_EVT         5       /* stream data transfer stopped */
#define BSA_AVK_RC_OPEN_EVT      8       /* remote control channel open */
#define BSA_AVK_RC_PEER_OPEN_EVT 9       /* remote control channel open by peer */
#define BSA_AVK_RC_CLOSE_EVT     10      /* remote control channel closed */
#define BSA_AVK_REMOTE_RSP_EVT   11      /* remote control response */
#define BSA_AVK_VENDOR_CMD_EVT   12      /* vendor dependent remote control command */
#define BSA_AVK_VENDOR_RSP_EVT   13      /* vendor dependent remote control response */
#define BSA_AVK_CP_INFO_EVT      18      /* content protection message */

#define BSA_AVK_REGISTER_NOTIFICATION_EVT   19      /* reg notfn response */
#define BSA_AVK_LIST_PLAYER_APP_ATTR_EVT    20      /* list player attr response */
#define BSA_AVK_LIST_PLAYER_APP_VALUES_EVT  21      /* list player value response */
#define BSA_AVK_SET_PLAYER_APP_VALUE_EVT    22      /* set player value response */
#define BSA_AVK_GET_PLAYER_APP_VALUE_EVT    23      /* get player value response */
#define BSA_AVK_GET_PLAYER_ATTR_TEXT_EVT    24      /* get player attr text response */
#define BSA_AVK_GET_PLAYER_ATTR_VALUE_EVT   25      /* get player value text response */
#define BSA_AVK_GET_ELEM_ATTR_EVT           26      /* get element attrubute response */
#define BSA_AVK_GET_PLAY_STATUS_EVT         27      /* get plays status response */
#define BSA_AVK_SET_ABSOLUTE_VOLUME_EVT     28      /* set abs vol esponse */
#define BSA_AVK_SET_ADDRESSED_PLAYER_EVT    29      /* set addressed player response */
#define BSA_AVK_SET_BROWSED_PLAYER_EVT      30      /* set browsed player response */
#define BSA_AVK_GET_FOLDER_ITEMS_EVT        31      /* get folder items response */
#define BSA_AVK_CHANGE_PATH_EVT             32      /* change path response */
#define BSA_AVK_GET_ITEM_ATTR_EVT           33      /* get item attr response */
#define BSA_AVK_PLAY_ITEM_EVT               34      /* play item response */
#define BSA_AVK_ADD_TO_NOW_PLAYING_EVT      36      /* add to now playing response */
#define BSA_AVK_SET_ABS_VOL_CMD_EVT         37      /* set abs vol command */
#define BSA_AVK_REG_NOTIFICATION_CMD_EVT    38      /* reg notification cmd */


#define BTM_BLE_SEC_NONE                0
#define BTM_BLE_SEC_ENCRYPT             1 /* encrypt the link using current key */
#define BTM_BLE_SEC_ENCRYPT_NO_MITM     2
#define BTM_BLE_SEC_ENCRYPT_MITM        3

#define BT_TRANSPORT_BR_EDR    		1
#define BT_TRANSPORT_LE        		2

/* Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */
#define MAX_UUID_SIZE			16

/*   GATT_READ_MULTIPLE request data
*/
#define GATT_MAX_READ_MULTI_HANDLES      10           /* Max attributes to read in one request */

#define BTA_DM_BLE_SEC_NONE		BTM_BLE_SEC_NONE
#define BTA_DM_BLE_SEC_ENCRYPT		BTM_BLE_SEC_ENCRYPT
#define BTA_DM_BLE_SEC_NO_MITM		BTM_BLE_SEC_ENCRYPT_NO_MITM
#define BTA_DM_BLE_SEC_MITM		BTM_BLE_SEC_ENCRYPT_MITM
#define BTA_GATTC_MULTI_MAX		GATT_MAX_READ_MULTI_HANDLES

/* type of protocol mode */
#define BSA_HH_PROTO_RPT_MODE		BTA_HH_PROTO_RPT_MODE
#define BSA_HH_PROTO_BOOT_MODE		BTA_HH_PROTO_BOOT_MODE
#define BSA_HH_PROTO_UNKNOWN		BTA_HH_PROTO_UNKNOWN

typedef tBTA_HH_PROTO_MODE tBSA_HH_PROTO_MODE;
typedef UINT8   tBTM_BLE_SEC_ACT;
typedef UINT8   tGATT_IF;
typedef UINT8   tBTA_GATTC_ATTR_TYPE;
typedef UINT16  tBTA_GATT_REASON;
typedef UINT16  tBTA_GATT_PERM;
typedef UINT8   tBTA_GATT_CHAR_PROP;
typedef UINT8   tBTA_GATT_TRANSPORT;
typedef UINT8   tBTA_GATTC_WRITE_TYPE;
typedef UINT8   tBT_TRANSPORT;
typedef UINT8   tGATT_TRANSPORT;
typedef UINT32  tBSA_DM_CONFIG_MASK;
typedef UINT32  tBSA_DM_BLE_AD_MASK;
typedef UINT8 tBSA_DM_LE_ADDR_TYPE;

typedef struct
{
	tBSA_DM_LE_ADDR_TYPE   type;
	BD_ADDR             bd_addr;
} tBSA_DM_BLE_BD_ADDR;

/* set adv parameter for BLE advertising */
typedef struct
{
	UINT16 adv_int_min;             /* Minimum Advertisement interval */
	UINT16 adv_int_max;             /* Maximum Advertisement interval */
	tBSA_DM_BLE_BD_ADDR dir_bda;       /* Directed Adv BLE addr type and addr */
} tBSA_DM_BLE_ADV_PARAM;

/* BLE Preferred connection parameter */
typedef struct
{
	BD_ADDR bd_addr;            /* BdAddr of the peer device */
	UINT16 min_conn_int;        /* Preferred minimum connection interval */
	UINT16 max_conn_int;        /* Preferred maximum connection interval */
	UINT16 slave_latency;       /* Preferred slave latency */
	UINT16 supervision_tout;    /* Preferred supervision timeout value */
	BOOLEAN is_immediate_updating; /*If TRUE,send udpate conn param request(slave) or update param to controller(master).*/
					/*Otherwise, set preferred value for master*/
} tBSA_DM_BLE_CONN_PARAM;


enum {
	BTA_GATTC_ATTR_TYPE_INCL_SRVC,
	BTA_GATTC_ATTR_TYPE_CHAR,
	BTA_GATTC_ATTR_TYPE_CHAR_DESCR,
	BTA_GATTC_ATTR_TYPE_SRVC
};

typedef struct {
#define LEN_UUID_16     2
#define LEN_UUID_32     4
#define LEN_UUID_128    16
	UINT16          len;
	union
	{
		UINT16  uuid16;
		UINT32  uuid32;
		UINT8   uuid128[MAX_UUID_SIZE];
	} uu;
} tBT_UUID;

typedef struct {
	tBT_UUID  uuid;           /* uuid of the attribute */
	UINT8     inst_id;        /* instance ID */
} tBTA_GATT_ID;

typedef struct {
	tBTA_GATT_ID id;
	BOOLEAN      is_primary;
} tBTA_GATT_SRVC_ID;

typedef struct {
	tBTA_GATT_SRVC_ID srvc_id;
	tBTA_GATT_ID      char_id;
} tBTA_GATTC_CHAR_ID;

typedef struct {
	tBTA_GATTC_CHAR_ID char_id;
	tBTA_GATT_ID       descr_id;
} tBTA_GATTC_CHAR_DESCR_ID;

typedef struct {
	UINT8                       num_pres_fmt;   /* number of presentation format aggregated*/
	tBTA_GATTC_CHAR_DESCR_ID    pre_format[BTA_GATTC_MULTI_MAX];
} tBTA_GATT_CHAR_AGGRE_VALUE;

typedef struct {
	UINT16  len;
	UINT8   *p_value;
} tBTA_GATT_UNFMT;

typedef union {
	tBTA_GATT_CHAR_AGGRE_VALUE  aggre_value;
	tBTA_GATT_UNFMT             unformat;

} tBTA_GATT_READ_VAL;

typedef struct {
	UINT16  low;
	UINT16  hi;
} tBSA_DM_BLE_INT_RANGE;

typedef struct {
	UINT8       adv_type;
	UINT8       len;
	UINT8       *p_val;     /* number of len byte */
} tBTA_BLE_PROP_ELEM;

/* vendor proprietary adv type */
typedef struct {
	UINT8                   num_elem;
	tBTA_BLE_PROP_ELEM      *p_elem;
} tBSA_DM_BLE_PROPRIETARY;

typedef struct {
	BOOLEAN list_cmpl;
	UINT8 uuid128[MAX_UUID_SIZE];
} tBSA_DM_BLE_128SERVICE;

/* BLE Advertisement configuration parameters */
typedef struct {
	UINT8                     len; /* Number of bytes of data to be advertised */
	UINT8                     flag; /* AD flag value to be set */
	UINT8                     p_val[BSA_DM_BLE_AD_DATA_LEN];/* Data to be advertised */
	tBSA_DM_BLE_AD_MASK       adv_data_mask; /* Data Mask: Eg: BTM_BLE_AD_BIT_FLAGS, BTM_BLE_AD_BIT_MANU */
	UINT16                    appearance_data;     /* Device appearance data */
	UINT8                     num_service; /* number of services */
	BOOLEAN                   is_part_service; /* is transfer part UUID16 services type flag or completely services type flag */
	UINT16                    uuid_val[BSA_DM_BLE_AD_UUID_MAX];
	/* for DataType Service Data - 0x16 */
	UINT8                     service_data_len; /* length = AD type + service data uuid + data) */
	tBT_UUID                  service_data_uuid; /* service data uuid */
	UINT8                     service_data_val[BSA_DM_BLE_AD_DATA_LEN];
	BOOLEAN                   is_scan_rsp;  /* is the data scan response or adv data */
	UINT8                     tx_power;
	tBSA_DM_BLE_INT_RANGE     int_range;
	tBSA_DM_BLE_128SERVICE    services_128b;
	tBSA_DM_BLE_128SERVICE    sol_service_128b;
	tBSA_DM_BLE_PROPRIETARY   elem;
} tBSA_DM_BLE_ADV_CONFIG;

typedef tGATT_IF                   tBSA_BLE_IF;
typedef tBTA_GATTC_CHAR_ID         tBSA_BLE_CL_CHAR_ID;
typedef tBTA_GATTC_CHAR_DESCR_ID   tBSA_BLE_CL_CHAR_DESCR_ID;
typedef tBTA_GATT_ID               tBSA_BLE_ID;
typedef tBTA_GATT_READ_VAL         tBSA_BLE_READ_VAL;
typedef tBTA_GATT_REASON           tBSA_BLE_REASON;
typedef tBTA_GATT_PERM             tBSA_BLE_PERM;
typedef tBTA_GATT_CHAR_PROP        tBSA_BLE_CHAR_PROP;
typedef tBTA_GATT_TRANSPORT        tBSA_BLE_TRANSPORT;
typedef tBTA_GATTC_WRITE_TYPE      tBSA_BLE_WRITE_TYPE;
typedef tBTM_BLE_SEC_ACT           tBTA_DM_BLE_SEC_ACT;


/* Attribute permissions
*/
#define GATT_PERM_READ              		(1 << 0) /* bit 0 */
#define GATT_PERM_READ_ENCRYPTED    		(1 << 1) /* bit 1 */
#define GATT_PERM_READ_ENC_MITM     		(1 << 2) /* bit 2 */
#define GATT_PERM_WRITE             		(1 << 4) /* bit 4 */
#define GATT_PERM_WRITE_ENCRYPTED   		(1 << 5) /* bit 5 */
#define GATT_PERM_WRITE_ENC_MITM    		(1 << 6) /* bit 6 */
#define GATT_PERM_WRITE_SIGNED      		(1 << 7) /* bit 7 */
#define GATT_PERM_WRITE_SIGNED_MITM 		(1 << 8) /* bit 8 */

#define BTA_GATT_PERM_READ              	GATT_PERM_READ              /* bit 0 -  0x0001 */
#define BTA_GATT_PERM_READ_ENCRYPTED    	GATT_PERM_READ_ENCRYPTED    /* bit 1 -  0x0002 */
#define BTA_GATT_PERM_READ_ENC_MITM     	GATT_PERM_READ_ENC_MITM     /* bit 2 -  0x0004 */
#define BTA_GATT_PERM_WRITE             	GATT_PERM_WRITE             /* bit 4 -  0x0010 */
#define BTA_GATT_PERM_WRITE_ENCRYPTED   	GATT_PERM_WRITE_ENCRYPTED   /* bit 5 -  0x0020 */
#define BTA_GATT_PERM_WRITE_ENC_MITM    	GATT_PERM_WRITE_ENC_MITM    /* bit 6 -  0x0040 */
#define BTA_GATT_PERM_WRITE_SIGNED      	GATT_PERM_WRITE_SIGNED      /* bit 7 -  0x0080 */
#define BTA_GATT_PERM_WRITE_SIGNED_MITM 	GATT_PERM_WRITE_SIGNED_MITM /* bit 8 -  0x0100 */

#define BSA_GATT_PERM_READ              	BTA_GATT_PERM_READ              /* bit 0 -  0x0001 */
#define BSA_GATT_PERM_READ_ENCRYPTED    	BTA_GATT_PERM_READ_ENCRYPTED    /* bit 1 -  0x0002 */
#define BSA_GATT_PERM_READ_ENC_MITM     	BTA_GATT_PERM_READ_ENC_MITM     /* bit 2 -  0x0004 */
#define BSA_GATT_PERM_WRITE             	BTA_GATT_PERM_WRITE             /* bit 4 -  0x0010 */
#define BSA_GATT_PERM_WRITE_ENCRYPTED   	BTA_GATT_PERM_WRITE_ENCRYPTED   /* bit 5 -  0x0020 */
#define BSA_GATT_PERM_WRITE_ENC_MITM    	BTA_GATT_PERM_WRITE_ENC_MITM    /* bit 6 -  0x0040 */
#define BSA_GATT_PERM_WRITE_SIGNED      	BTA_GATT_PERM_WRITE_SIGNED      /* bit 7 -  0x0080 */
#define BSA_GATT_PERM_WRITE_SIGNED_MITM 	BTA_GATT_PERM_WRITE_SIGNED_MITM /* bit 8 -  0x0100 */
/* End Attribute permissions */

/* Definition of characteristic properties */
#define GATT_CHAR_PROP_BIT_BROADCAST    	(1 << 0)
#define GATT_CHAR_PROP_BIT_READ         	(1 << 1)
#define GATT_CHAR_PROP_BIT_WRITE_NR     	(1 << 2)
#define GATT_CHAR_PROP_BIT_WRITE        	(1 << 3)
#define GATT_CHAR_PROP_BIT_NOTIFY       	(1 << 4)
#define GATT_CHAR_PROP_BIT_INDICATE     	(1 << 5)
#define GATT_CHAR_PROP_BIT_AUTH         	(1 << 6)
#define GATT_CHAR_PROP_BIT_EXT_PROP     	(1 << 7)

#define BTA_GATT_CHAR_PROP_BIT_BROADCAST	GATT_CHAR_PROP_BIT_BROADCAST    /* 0x01 */
#define BTA_GATT_CHAR_PROP_BIT_READ		GATT_CHAR_PROP_BIT_READ    /* 0x02 */
#define BTA_GATT_CHAR_PROP_BIT_WRITE_NR		GATT_CHAR_PROP_BIT_WRITE_NR    /* 0x04 */
#define BTA_GATT_CHAR_PROP_BIT_WRITE		GATT_CHAR_PROP_BIT_WRITE       /* 0x08 */
#define BTA_GATT_CHAR_PROP_BIT_NOTIFY		GATT_CHAR_PROP_BIT_NOTIFY      /* 0x10 */
#define BTA_GATT_CHAR_PROP_BIT_INDICATE		GATT_CHAR_PROP_BIT_INDICATE    /* 0x20 */
#define BTA_GATT_CHAR_PROP_BIT_AUTH		GATT_CHAR_PROP_BIT_AUTH        /* 0x40 */
#define BTA_GATT_CHAR_PROP_BIT_EXT_PROP		GATT_CHAR_PROP_BIT_EXT_PROP    /* 0x80 */

#define BSA_GATT_CHAR_PROP_BIT_BROADCAST	BTA_GATT_CHAR_PROP_BIT_BROADCAST   /* 0x01 */
#define BSA_GATT_CHAR_PROP_BIT_READ		BTA_GATT_CHAR_PROP_BIT_READ        /* 0x02 */
#define BSA_GATT_CHAR_PROP_BIT_WRITE_NR		BTA_GATT_CHAR_PROP_BIT_WRITE_NR    /* 0x04 */
#define BSA_GATT_CHAR_PROP_BIT_WRITE		BTA_GATT_CHAR_PROP_BIT_WRITE       /* 0x08 */
#define BSA_GATT_CHAR_PROP_BIT_NOTIFY		BTA_GATT_CHAR_PROP_BIT_NOTIFY      /* 0x10 */
#define BSA_GATT_CHAR_PROP_BIT_INDICATE		BTA_GATT_CHAR_PROP_BIT_INDICATE    /* 0x20 */
#define BSA_GATT_CHAR_PROP_BIT_AUTH		BTA_GATT_CHAR_PROP_BIT_AUTH        /* 0x40 */
#define BSA_GATT_CHAR_PROP_BIT_EXT_PROP		BTA_GATT_CHAR_PROP_BIT_EXT_PROP    /* 0x80 */
/* End characteristic properties */

#define BSA_GATTC_ATTR_TYPE_INCL_SRVC		BTA_GATTC_ATTR_TYPE_INCL_SRVC
#define BSA_GATTC_ATTR_TYPE_CHAR		BTA_GATTC_ATTR_TYPE_CHAR
#define BSA_GATTC_ATTR_TYPE_CHAR_DESCR		BTA_GATTC_ATTR_TYPE_CHAR_DESCR
#define BSA_GATTC_ATTR_TYPE_SRVC		BTA_GATTC_ATTR_TYPE_SRVC
typedef tBTA_GATTC_ATTR_TYPE			tBSA_GATTC_ATTR_TYPE;

/* Max client application BSA BLE Client can support */
#ifndef BSA_BLE_CLIENT_MAX
#define BSA_BLE_CLIENT_MAX    			3
#endif

/* Max server application BSA BLE Server can support */
#define BSA_BLE_SERVER_MAX    			4
#define BSA_BLE_ATTRIBUTE_MAX 			20

#ifndef BSA_BLE_SERVER_SECURITY
#define BSA_BLE_SERVER_SECURITY BTA_DM_BLE_SEC_NONE
#endif

#define BSA_BLE_INVALID_IF         		0xff
#define BSA_BLE_INVALID_CONN       		0xffff

/* Define common 16-bit service class UUIDs
*/
#define UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER 0X1000
#define UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR  0X1001
#define UUID_SERVCLASS_PUBLIC_BROWSE_GROUP      0X1002
#define UUID_SERVCLASS_SERIAL_PORT              0X1101
#define UUID_SERVCLASS_LAN_ACCESS_USING_PPP     0X1102
#define UUID_SERVCLASS_DIALUP_NETWORKING        0X1103
#define UUID_SERVCLASS_IRMC_SYNC                0X1104
#define UUID_SERVCLASS_OBEX_OBJECT_PUSH         0X1105
#define UUID_SERVCLASS_OBEX_FILE_TRANSFER       0X1106
#define UUID_SERVCLASS_IRMC_SYNC_COMMAND        0X1107
#define UUID_SERVCLASS_HEADSET                  0X1108
#define UUID_SERVCLASS_CORDLESS_TELEPHONY       0X1109
#define UUID_SERVCLASS_AUDIO_SOURCE             0X110A
#define UUID_SERVCLASS_AUDIO_SINK               0X110B
#define UUID_SERVCLASS_AV_REM_CTRL_TARGET       0X110C  /* Audio/Video Control profile */
#define UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION   0X110D  /* Advanced Audio Distribution profile */
#define UUID_SERVCLASS_AV_REMOTE_CONTROL        0X110E  /* Audio/Video Control profile */
#define UUID_SERVCLASS_AV_REM_CTRL_CONTROL      0X110F  /* Audio/Video Control profile */
#define UUID_SERVCLASS_INTERCOM                 0X1110
#define UUID_SERVCLASS_FAX                      0X1111
#define UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY    0X1112
#define UUID_SERVCLASS_WAP                      0X1113
#define UUID_SERVCLASS_WAP_CLIENT               0X1114
#define UUID_SERVCLASS_PANU                     0X1115  /* PAN profile */
#define UUID_SERVCLASS_NAP                      0X1116  /* PAN profile */
#define UUID_SERVCLASS_GN                       0X1117  /* PAN profile */
#define UUID_SERVCLASS_DIRECT_PRINTING          0X1118  /* BPP profile */
#define UUID_SERVCLASS_REFERENCE_PRINTING       0X1119  /* BPP profile */
#define UUID_SERVCLASS_IMAGING                  0X111A  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_RESPONDER        0X111B  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE     0X111C  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_REF_OBJECTS      0X111D  /* Imaging profile */
#define UUID_SERVCLASS_HF_HANDSFREE             0X111E  /* Handsfree profile */
#define UUID_SERVCLASS_AG_HANDSFREE             0X111F  /* Handsfree profile */
#define UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE  0X1120  /* BPP profile */
#define UUID_SERVCLASS_REFLECTED_UI             0X1121  /* BPP profile */
#define UUID_SERVCLASS_BASIC_PRINTING           0X1122  /* BPP profile */
#define UUID_SERVCLASS_PRINTING_STATUS          0X1123  /* BPP profile */
#define UUID_SERVCLASS_HUMAN_INTERFACE          0X1124  /* HID profile */
#define UUID_SERVCLASS_CABLE_REPLACEMENT        0X1125  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_PRINT               0X1126  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_SCAN                0X1127  /* HCRP profile */
#define UUID_SERVCLASS_COMMON_ISDN_ACCESS       0X1128  /* CAPI Message Transport Protocol*/
#define UUID_SERVCLASS_VIDEO_CONFERENCING_GW    0X1129  /* Video Conferencing profile */
#define UUID_SERVCLASS_UDI_MT                   0X112A  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_UDI_TA                   0X112B  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_VCP                      0X112C  /* Video Conferencing profile */
#define UUID_SERVCLASS_SAP                      0X112D  /* SIM Access profile */
#define UUID_SERVCLASS_PBAP_PCE                 0X112E  /* Phonebook Access - PCE */
#define UUID_SERVCLASS_PBAP_PSE                 0X112F  /* Phonebook Access - PSE */
#define UUID_SERVCLASS_PHONE_ACCESS             0x1130
#define UUID_SERVCLASS_HEADSET_HS               0x1131  /* Headset - HS, from HSP v1.2 */
#define UUID_SERVCLASS_3DD                      0x1137  /* 3D Sync (Display role) */
#define UUID_SERVCLASS_3DG                      0x1138  /* 3D Sync (Glasses role) */
#define UUID_SERVCLASS_3DS                      0x1139  /* 3D Sync Profile */
#define UUID_SERVCLASS_PNP_INFORMATION          0X1200  /* Device Identification */
#define UUID_SERVCLASS_GENERIC_NETWORKING       0X1201
#define UUID_SERVCLASS_GENERIC_FILETRANSFER     0X1202
#define UUID_SERVCLASS_GENERIC_AUDIO            0X1203
#define UUID_SERVCLASS_GENERIC_TELEPHONY        0X1204
#define UUID_SERVCLASS_UPNP_SERVICE             0X1205  /* UPNP_Service [ESDP] */
#define UUID_SERVCLASS_UPNP_IP_SERVICE          0X1206  /* UPNP_IP_Service [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_PAN         0X1300  /* UPNP_IP_PAN [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_LAP         0X1301  /* UPNP_IP_LAP [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP       0X1302  /* UPNP_L2CAP [ESDP] */
#define UUID_SERVCLASS_VIDEO_SOURCE             0X1303  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_SINK               0X1304  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_DISTRIBUTION       0X1305  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_HDP_PROFILE              0X1400  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SOURCE               0X1401  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SINK                 0X1402  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_MAP_PROFILE              0X1134  /* MAP profile UUID */
#define UUID_SERVCLASS_MESSAGE_ACCESS           0X1132  /* Message Access Service UUID */
#define UUID_SERVCLASS_MESSAGE_NOTIFICATION     0X1133  /* Message Notification Service UUID */

#define UUID_SERVCLASS_GAP_SERVER               0x1800
#define UUID_SERVCLASS_GATT_SERVER              0x1801
#define UUID_SERVCLASS_IMMEDIATE_ALERT          0x1802      /* immediate alert */
#define UUID_SERVCLASS_LINKLOSS                 0x1803      /* Link Loss Alert */
#define UUID_SERVCLASS_TX_POWER                 0x1804      /* TX power */
#define UUID_SERVCLASS_CURRENT_TIME             0x1805      /* Link Loss Alert */
#define UUID_SERVCLASS_DST_CHG                  0x1806      /* DST Time change */
#define UUID_SERVCLASS_REF_TIME_UPD             0x1807      /* reference time update */
#define UUID_SERVCLASS_THERMOMETER              0x1809      /* Thermometer UUID */
#define UUID_SERVCLASS_DEVICE_INFO              0x180A      /* device info service */
#define UUID_SERVCLASS_NWA                      0x180B      /* Network availability */
#define UUID_SERVCLASS_HEART_RATE               0x180D      /* Heart Rate service */
#define UUID_SERVCLASS_PHALERT                  0x180E      /* phone alert service */
#define UUID_SERVCLASS_BATTERY                  0x180F     /* battery service */
#define UUID_SERVCLASS_BPM                      0x1810      /*  blood pressure service */
#define UUID_SERVCLASS_ALERT_NOTIFICATION       0x1811      /* alert notification service */
#define UUID_SERVCLASS_LE_HID                   0x1812     /*  HID over LE */
#define UUID_SERVCLASS_SCAN_PARAM               0x1813      /* Scan Parameter service */
#define UUID_SERVCLASS_GLUCOSE                  0x1808      /* Glucose Meter Service */
#define UUID_SERVCLASS_RSC                      0x1814      /* Runners Speed and Cadence Service */
#define UUID_SERVCLASS_CSC                      0x1816      /* Cycling Speed and Cadence Service */
#define UUID_SERVCLASS_BAV_SOURCE               0x8000      /* Broadcast AV service*/ /* BSA_SPECIFIC */
#define UUID_SERVCLASS_CP                       0x1818      /* Cycling Power Service (pre-Adoption IOP) */
#define UUID_SERVCLASS_LN                       0x1819      /* Location and Navigation Service (pre-Adoption IOP) */

#define UUID_SERVCLASS_TEST_SERVER              0x9000      /* Test Group UUID */

#define BSA_BLE_UUID_SERVCLASS_GAP_SERVER                    UUID_SERVCLASS_GAP_SERVER
#define BSA_BLE_UUID_SERVCLASS_GATT_SERVER                   UUID_SERVCLASS_GATT_SERVER
#define BSA_BLE_UUID_SERVCLASS_IMMEDIATE_ALERT               UUID_SERVCLASS_IMMEDIATE_ALERT
#define BSA_BLE_UUID_SERVCLASS_LINKLOSS                      UUID_SERVCLASS_LINKLOSS
#define BSA_BLE_UUID_SERVCLASS_TX_POWER                      UUID_SERVCLASS_TX_POWER
#define BSA_BLE_UUID_SERVCLASS_CURRENT_TIME                  UUID_SERVCLASS_CURRENT_TIME
#define BSA_BLE_UUID_SERVCLASS_DST_CHG                       UUID_SERVCLASS_DST_CHG
#define BSA_BLE_UUID_SERVCLASS_REF_TIME_UPD                  UUID_SERVCLASS_REF_TIME_UPD
#define BSA_BLE_UUID_SERVCLASS_GLUCOSE                       UUID_SERVCLASS_GLUCOSE
#define BSA_BLE_UUID_SERVCLASS_HEALTH_THERMOMETER            UUID_SERVCLASS_THERMOMETER
#define BSA_BLE_UUID_SERVCLASS_DEVICE_INFORMATION            UUID_SERVCLASS_DEVICE_INFO
#define BSA_BLE_UUID_SERVCLASS_NWA                           UUID_SERVCLASS_NWA
#define BSA_BLE_UUID_SERVCLASS_PHALERT                       UUID_SERVCLASS_PHALERT
#define BSA_BLE_UUID_SERVCLASS_HEART_RATE                    UUID_SERVCLASS_HEART_RATE
#define BSA_BLE_UUID_SERVCLASS_BATTERY_SERVICE               UUID_SERVCLASS_BATTERY
#define BSA_BLE_UUID_SERVCLASS_BLOOD_PRESSURE                UUID_SERVCLASS_BPM
#define BSA_BLE_UUID_SERVCLASS_ALERT_NOTIFICATION_SERVICE    UUID_SERVCLASS_ALERT_NOTIFICATION
#define BSA_BLE_UUID_SERVCLASS_HUMAN_INTERFACE_DEVICE        UUID_SERVCLASS_LE_HID
#define BSA_BLE_UUID_SERVCLASS_SCAN_PARAMETERS               UUID_SERVCLASS_SCAN_PARAM
#define BSA_BLE_UUID_SERVCLASS_RUNNING_SPEED_AND_CADENCE     UUID_SERVCLASS_RSC
#define BSA_BLE_UUID_SERVCLASS_CYCLING_SPEED_AND_CADENCE     UUID_SERVCLASS_CSC
#define BSA_BLE_UUID_SERVCLASS_TEST_SERVER                   UUID_SERVCLASS_TEST_SERVER

/* GATT attribute types
*/
#define GATT_UUID_PRI_SERVICE           	0x2800
#define GATT_UUID_SEC_SERVICE           	0x2801
#define GATT_UUID_INCLUDE_SERVICE       	0x2802
#define GATT_UUID_CHAR_DECLARE          	0x2803      /*  Characteristic Declaration*/

#define GATT_UUID_CHAR_EXT_PROP         	0x2900      /*	Characteristic Extended Properties */
#define GATT_UUID_CHAR_DESCRIPTION      	0x2901      /*  Characteristic User Description*/
#define GATT_UUID_CHAR_CLIENT_CONFIG    	0x2902      /*  Client Characteristic Configuration */
#define GATT_UUID_CHAR_SRVR_CONFIG      	0x2903      /*  Server Characteristic Configuration */
#define GATT_UUID_CHAR_PRESENT_FORMAT   	0x2904      /*  Characteristic Presentation Format*/
#define GATT_UUID_CHAR_AGG_FORMAT       	0x2905      /*  Characteristic Aggregate Format*/
#define GATT_UUID_CHAR_VALID_RANGE       	0x2906      /*  Characteristic Valid Range */
#define GATT_UUID_EXT_RPT_REF_DESCR     	0x2907
#define GATT_UUID_RPT_REF_DESCR         	0x2908

/* GAP Profile Attributes
*/
#define GATT_UUID_GAP_DEVICE_NAME       	0x2A00
#define GATT_UUID_GAP_ICON              	0x2A01
#define GATT_UUID_GAP_PREF_CONN_PARAM   	0x2A04
#define GATT_UUID_GAP_CENTRAL_ADDR_RESOL    	0x2AA6

/* Attribute Profile Attribute UUID */
#define GATT_UUID_GATT_SRV_CHGD         	0x2A05
/* Attribute Protocol Test */

/* Link Loss Service */
#define GATT_UUID_ALERT_LEVEL           	0x2A06      /* Alert Level */
#define GATT_UUID_TX_POWER_LEVEL        	0x2A07      /* TX power level */

/* Time Profile */
/* Current Time Service */
#define GATT_UUID_CURRENT_TIME          	0x2A2B      /* Current Time */
#define GATT_UUID_LOCAL_TIME_INFO       	0x2A0F      /* Local time info */
#define GATT_UUID_REF_TIME_INFO         	0x2A14      /* reference time information */

/* NwA Profile */
#define GATT_UUID_NW_STATUS             	0x2A18      /* network availability status */
#define GATT_UUID_NW_TRIGGER            	0x2A1A      /* Network availability trigger */

/* phone alert */
#define GATT_UUID_ALERT_STATUS          	0x2A40    /* alert status */
#define GATT_UUID_RINGER_CP             	0x2A42    /* ringer control point */
#define GATT_UUID_RINGER_SETTING        	0x2A41    /* ringer setting */

/* Glucose Service */
#define GATT_UUID_GM_MEASUREMENT        	0x2A18
#define GATT_UUID_GM_CONTEXT            	0x2A34
#define GATT_UUID_GM_CONTROL_POINT      	0x2A52
#define GATT_UUID_GM_FEATURE            	0x2A51

/* device infor characteristic */
#define GATT_UUID_SYSTEM_ID             	0x2A23
#define GATT_UUID_MODEL_NUMBER_STR      	0x2A24
#define GATT_UUID_SERIAL_NUMBER_STR     	0x2A25
#define GATT_UUID_FW_VERSION_STR        	0x2A26
#define GATT_UUID_HW_VERSION_STR        	0x2A27
#define GATT_UUID_SW_VERSION_STR        	0x2A28
#define GATT_UUID_MANU_NAME             	0x2A29
#define GATT_UUID_IEEE_DATA             	0x2A2A
#define GATT_UUID_PNP_ID                	0x2A50

/* HID characteristics */
#define GATT_UUID_HID_INFORMATION       	0x2A4A
#define GATT_UUID_HID_REPORT_MAP        	0x2A4B
#define GATT_UUID_HID_CONTROL_POINT     	0x2A4C
#define GATT_UUID_HID_REPORT            	0x2A4D
#define GATT_UUID_HID_PROTO_MODE        	0x2A4E
#define GATT_UUID_HID_BT_KB_INPUT       	0x2A22
#define GATT_UUID_HID_BT_KB_OUTPUT      	0x2A32
#define GATT_UUID_HID_BT_MOUSE_INPUT    	0x2A33

/* Battery Service char */
#define GATT_UUID_BATTERY_LEVEL         	0x2A19

#define GATT_UUID_SC_CONTROL_POINT      	0x2A55
#define GATT_UUID_SENSOR_LOCATION       	0x2A5D

/* RUNNERS SPEED AND CADENCE SERVICE */
#define GATT_UUID_RSC_MEASUREMENT       	0x2A53
#define GATT_UUID_RSC_FEATURE           	0x2A54

/* CYCLING SPEED AND CADENCE SERVICE */
#define GATT_UUID_CSC_MEASUREMENT       	0x2A5B
#define GATT_UUID_CSC_FEATURE           	0x2A5C

/* CYCLING POWER SERVICE  (Temp for IOP) */
#define GATT_UUID_CP_MEASUREMENT        	0x2A63
#define GATT_UUID_CP_VECTOR             	0x2A64
#define GATT_UUID_CP_FEATURE            	0x2A65
#define GATT_UUID_CP_CONTROL_POINT      	0x2A66

/* LOCATION AND NAVIGATION SERVICE  (Temp for IOP) */
#define GATT_UUID_LN_LOC_AND_SPEED      	0x2A67
#define GATT_UUID_LN_NAVIGATION         	0x2A68
#define GATT_UUID_LN_POSITION_QUALITY   	0x2A69
#define GATT_UUID_LN_FEATURE            	0x2A6A
#define GATT_UUID_LN_CONTROL_POINT      	0x2A6B

/* Scan Parameter charatceristics */
#define GATT_UUID_SCAN_INT_WINDOW       	0x2A4F
#define GATT_UUID_SCAN_REFRESH          	0x2A31

/* Transports for the primary service  */
#define GATT_TRANSPORT_LE           		BT_TRANSPORT_LE
#define GATT_TRANSPORT_BR_EDR       		BT_TRANSPORT_BR_EDR
#define GATT_TRANSPORT_LE_BR_EDR    		(BT_TRANSPORT_LE|BT_TRANSPORT_BR_EDR)

#define BSA_BLE_GATT_UUID_PRI_SERVICE                       GATT_UUID_PRI_SERVICE
#define BSA_BLE_GATT_UUID_SEC_SERVICE                       GATT_UUID_SEC_SERVICE
#define BSA_BLE_GATT_UUID_INCLUDE_SERVICE                   GATT_UUID_INCLUDE_SERVICE
#define BSA_BLE_GATT_UUID_CHAR_DECLARE                      GATT_UUID_CHAR_DECLARE            /*  Characteristic Declaration*/
#define BSA_BLE_GATT_UUID_CHAR_EXT_PROP                     GATT_UUID_CHAR_EXT_PROP           /*  Characteristic Extended Properties */
#define BSA_BLE_GATT_UUID_CHAR_DESCRIPTION                  GATT_UUID_CHAR_DESCRIPTION        /*  Characteristic User Description*/
#define BSA_BLE_GATT_UUID_CHAR_CLIENT_CONFIG                GATT_UUID_CHAR_CLIENT_CONFIG      /*  Client Characteristic Configuration */
#define BSA_BLE_GATT_UUID_CHAR_VALID_RANGE                  GATT_UUID_CHAR_VALID_RANGE        /*  Characteristic Valid Range */

#define BSA_BLE_GATT_UUID_CHAR_CLIENT_CONFIG_ENABLE_NOTI    0x01    /* Enable Notification of Client Characteristic Configuration, defined at bluetooth org */
#define BSA_BLE_GATT_UUID_CHAR_CLIENT_CONFIG_ENABLE_INDI    0x02    /* Enable Indication of Client Characteristic Configuration, defined at bluetooth org */

#define BSA_BLE_GATT_UUID_CHAR_SRVR_CONFIG                  GATT_UUID_CHAR_SRVR_CONFIG        /*  Server Characteristic Configuration */
#define BSA_BLE_GATT_UUID_CHAR_PRESENT_FORMAT               GATT_UUID_CHAR_PRESENT_FORMAT     /*  Characteristic Presentation Format*/
#define BSA_BLE_GATT_UUID_CHAR_AGG_FORMAT                   GATT_UUID_CHAR_AGG_FORMAT         /*  Characteristic Aggregate Format*/
#define BSA_BLE_GATT_UUID_CHAR_VALID_RANGE                  GATT_UUID_CHAR_VALID_RANGE        /*  Characteristic Valid Range */

#define BSA_BLE_GATT_UUID_GAP_DEVICE_NAME                   GATT_UUID_GAP_DEVICE_NAME
#define BSA_BLE_GATT_UUID_GAP_ICON                          GATT_UUID_GAP_ICON
#define BSA_BLE_GATT_UUID_GAP_PRIVACY_FLAG                  GATT_UUID_GAP_PRIVACY_FLAG
#define BSA_BLE_GATT_UUID_GAP_RECONN_ADDR                   GATT_UUID_GAP_RECONN_ADDR
#define BSA_BLE_GATT_UUID_GAP_PREF_CONN_PARAM               GATT_UUID_GAP_PREF_CONN_PARAM

#define BSA_BLE_GATT_UUID_SENSOR_LOCATION                   GATT_UUID_SENSOR_LOCATION

/* Battery Service */
#define BSA_BLE_GATT_UUID_BATTERY_LEVEL                     GATT_UUID_BATTERY_LEVEL

/* device infor characteristic */
#define BSA_BLE_GATT_UUID_SYSTEM_ID                         GATT_UUID_SYSTEM_ID
#define BSA_BLE_GATT_UUID_MODEL_NUMBER_STR                  GATT_UUID_MODEL_NUMBER_STR
#define BSA_BLE_GATT_UUID_SERIAL_NUMBER_STR                 GATT_UUID_SERIAL_NUMBER_STR
#define BSA_BLE_GATT_UUID_FW_VERSION_STR                    GATT_UUID_FW_VERSION_STR
#define BSA_BLE_GATT_UUID_HW_VERSION_STR                    GATT_UUID_HW_VERSION_STR
#define BSA_BLE_GATT_UUID_SW_VERSION_STR                    GATT_UUID_SW_VERSION_STR
#define BSA_BLE_GATT_UUID_MANU_NAME                         GATT_UUID_MANU_NAME
#define BSA_BLE_GATT_UUID_IEEE_DATA                         GATT_UUID_IEEE_DATA
#define BSA_BLE_GATT_UUID_PNP_ID                            GATT_UUID_PNP_ID

/* Link Loss Service */
#define BSA_BLE_GATT_UUID_ALERT_LEVEL                       GATT_UUID_ALERT_LEVEL      /* Alert Level */
#define BSA_BLE_GATT_UUID_TX_POWER_LEVEL                    GATT_UUID_TX_POWER_LEVEL      /* TX power level */

/* Heart Rate Service */
#define BSA_BLE_GATT_UUID_HEART_RATE_MEASUREMENT            0x2A37
#define BSA_BLE_GATT_UUID_BODY_SENSOR_LOCATION              0x2A38
#define BSA_BLE_GATT_UUID_BODY_SENSOR_CONTROL_POINT         0x2A39

/* BLOOD PRESSURE SERVICE */
#define BSA_BLE_GATT_UUID_BLOOD_PRESSURE_FEATURE            0x2A49
#define BSA_BLE_GATT_UUID_BLOOD_PRESSURE_MEASUREMENT        0x2A35
#define BSA_BLE_GATT_UUID_INTERMEDIATE_CUFF_PRESSURE        0x2A36
#define BSA_BLE_GATT_LENGTH_OF_BLOOD_PRESSURE_MEASUREMENT   13

/*HEALTH THERMOMETER SERVICE*/
#define BSA_BLE_GATT_UUID_TEMPERATURE_TYPE                  0x2A1D
#define BSA_BLE_GATT_UUID_TEMPERATURE_MEASUREMENT           0X2A1C
#define BSA_BLE_GATT_UUID_INTERMEDIATE_TEMPERATURE          0x2A1E
#define BSA_BLE_GATT_UUID_TEMPERATURE_MEASUREMENT_INTERVAL  0x2A21

/* CYCLING SPEED AND CADENCE SERVICE      */
#define BSA_BLE_GATT_UUID_CSC_MEASUREMENT                   GATT_UUID_CSC_MEASUREMENT
#define BSA_BLE_GATT_UUID_CSC_FEATURE                       GATT_UUID_CSC_FEATURE
#define BSA_BLE_GATT_LENGTH_OF_CSC_MEASUREMENT              11

/* RUNNERS SPEED AND CADENCE SERVICE      */
#define BSA_BLE_GATT_UUID_RSC_MEASUREMENT                   GATT_UUID_RSC_MEASUREMENT
#define BSA_BLE_GATT_UUID_RSC_FEATURE                       GATT_UUID_RSC_FEATURE
#define BSA_BLE_GATT_LENGTH_OF_RSC_MEASUREMENT              10

#define BSA_BLE_GATT_UUID_SC_CONTROL_POINT                  GATT_UUID_SC_CONTROL_POINT

/* HID characteristics */
#define BSA_BLE_GATT_UUID_HID_INFORMATION                   GATT_UUID_HID_INFORMATION
#define BSA_BLE_GATT_UUID_HID_REPORT_MAP                    GATT_UUID_HID_REPORT_MAP
#define BSA_BLE_GATT_UUID_HID_CONTROL_POINT                 GATT_UUID_HID_CONTROL_POINT
#define BSA_BLE_GATT_UUID_HID_REPORT                        GATT_UUID_HID_REPORT
#define BSA_BLE_GATT_UUID_HID_PROTO_MODE                    GATT_UUID_HID_PROTO_MODE
#define BSA_BLE_GATT_UUID_HID_BT_KB_INPUT                   GATT_UUID_HID_BT_KB_INPUT
#define BSA_BLE_GATT_UUID_HID_BT_KB_OUTPUT                  GATT_UUID_HID_BT_KB_OUTPUT
#define BSA_BLE_GATT_UUID_HID_BT_MOUSE_INPUT                GATT_UUID_HID_BT_MOUSE_INPUT

#define BSA_BLE_GATT_TRANSPORT_LE			    GATT_TRANSPORT_LE
#define BSA_BLE_GATT_TRANSPORT_BR_EDR      		    GATT_TRANSPORT_BR_EDR
#define BSA_BLE_GATT_TRANSPORT_LE_BR_EDR		    GATT_TRANSPORT_LE_BR_EDR

/* Define character set */
#define AVRC_CHAR_SET_SIZE                      2

/* Define the Media Attribute IDs
*/
#define AVRC_MEDIA_ATTR_ID_TITLE                 0x00000001
#define AVRC_MEDIA_ATTR_ID_ARTIST                0x00000002
#define AVRC_MEDIA_ATTR_ID_ALBUM                 0x00000003
#define AVRC_MEDIA_ATTR_ID_TRACK_NUM             0x00000004
#define AVRC_MEDIA_ATTR_ID_NUM_TRACKS            0x00000005
#define AVRC_MEDIA_ATTR_ID_GENRE                 0x00000006
#define AVRC_MEDIA_ATTR_ID_PLAYING_TIME          0x00000007        /* in miliseconds */
#define AVRC_MAX_NUM_MEDIA_ATTR_ID               7

/* ui keypress definition */
#define APP_OPC_KEY_PUSH        1
#define APP_OPC_KEY_EXCH        2
#define APP_OPC_KEY_PULL        3
#define APP_OPC_KEY_PUSH_FILE   4
#define APP_OPC_KEY_CLOSE       5
#define APP_OPC_KEY_DISC        6
#define APP_OPC_KEY_QUIT        9

/* Security Service Levels [bit mask] (BTM_SetSecurityLevel)
** Encryption should not be used without authentication
*/
#define BTM_SEC_NONE               0x0000 /* Nothing required */
#define BTM_SEC_IN_AUTHORIZE       0x0001 /* Inbound call requires authorization */
#define BTM_SEC_IN_AUTHENTICATE    0x0002 /* Inbound call requires authentication */
#define BTM_SEC_IN_ENCRYPT         0x0004 /* Inbound call requires encryption */
#define BTM_SEC_OUT_AUTHORIZE      0x0008 /* Outbound call requires authorization */
#define BTM_SEC_OUT_AUTHENTICATE   0x0010 /* Outbound call requires authentication */
#define BTM_SEC_OUT_ENCRYPT        0x0020 /* Outbound call requires encryption */

/* Security Setting Mask */
#define BTA_SEC_NONE            BTM_SEC_NONE                                         /* No security. */
#define BTA_SEC_AUTHORIZE       (BTM_SEC_IN_AUTHORIZE )                              /* Authorization required (only needed for out going connection )*/
#define BTA_SEC_AUTHENTICATE    (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_OUT_AUTHENTICATE) /* Authentication required. */
#define BTA_SEC_ENCRYPT         (BTM_SEC_IN_ENCRYPT | BTM_SEC_OUT_ENCRYPT)           /* Encryption required. */

#define BSA_SEC_NONE            BTA_SEC_NONE        /* No security/authentication needed (insecure) */
#define BSA_SEC_AUTHENTICATION  BTA_SEC_AUTHENTICATE/* Authentication/pairing needed*/
#define BSA_SEC_AUTHORIZATION   BTA_SEC_AUTHORIZE   /* Authorization needed to access services */
#define BSA_SEC_ENCRYPTION      BTA_SEC_ENCRYPT     /* Encryption needed */
typedef UINT8 tBTA_SEC;
typedef tBTA_SEC tBSA_SEC_AUTH;

/* Service ID Mask */
#define BTA_RES_SERVICE_MASK        0x00000001  /* Reserved */
#define BTA_SPP_SERVICE_MASK        0x00000002  /* Serial port profile. */
#define BTA_DUN_SERVICE_MASK        0x00000004  /* Dial-up networking profile. */
#define BTA_A2DP_SRC_SERVICE_MASK   0x00000008  /* A2DP Source */ /* BSA_SPECIFIC */
#define BTA_AVRCP_TG_SERVICE_MASK   0x00000010  /* AVRCP Target. */ /* BSA_SPECIFIC */
#define BTA_HSP_SERVICE_MASK        0x00000020  /* HSP AG role. */
#define BTA_HFP_SERVICE_MASK        0x00000040  /* HFP AG role */
#define BTA_OPP_SERVICE_MASK        0x00000080  /* Object push  */
#define BTA_FTP_SERVICE_MASK        0x00000100  /* File transfer */
#define BTA_CTP_SERVICE_MASK        0x00000200  /* Cordless Terminal */
#define BTA_ICP_SERVICE_MASK        0x00000400  /* Intercom Terminal */
#define BTA_SYNC_SERVICE_MASK       0x00000800  /* Synchronization */
#define BTA_BPP_SERVICE_MASK        0x00001000  /* Print server */
#define BTA_BIP_SERVICE_MASK        0x00002000  /* Basic Imaging */
#define BTA_PANU_SERVICE_MASK       0x00004000  /* PAN User */
#define BTA_NAP_SERVICE_MASK        0x00008000  /* PAN Network access point */
#define BTA_GN_SERVICE_MASK         0x00010000  /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_MASK        0x00020000  /* PAN Group Ad-hoc networks */
#define BTA_A2DP_SERVICE_MASK       0x00040000  /* Advanced audio distribution */
#define BTA_AVRCP_SERVICE_MASK      0x00080000  /* A/V remote control */
#define BTA_HID_SERVICE_MASK        0x00100000  /* HID */
#define BTA_VDP_SERVICE_MASK        0x00200000  /* Video distribution */
#define BTA_PBAP_SERVICE_MASK       0x00400000  /* Phone Book Server */
#define BTA_HSP_HS_SERVICE_MASK     0x00800000  /* HFP HS role */
#define BTA_HFP_HS_SERVICE_MASK     0x01000000  /* HSP HS role */
#define BTA_MAS_SERVICE_MASK        0x02000000  /* Message Access Profile */
#define BTA_MN_SERVICE_MASK         0x04000000  /* Message Notification Profile */
#define BTA_HL_SERVICE_MASK         0x08000000  /* Health Device Profile */
#define BTA_PCE_SERVICE_MASK        0x10000000  /* Phone Book Client */
#define BTA_3DS_SERVICE_MASK        0x20000000  /* 3D Sync service */
#define BTA_BLE_SERVICE_MASK        0x40000000  /* GATT based service */
#define BTA_HID_DEVICE_SERVICE_MASK 0x80000000  /* HID Device */ /* BSA_SPECIFIC */

#if BLE_INCLUDED == TRUE && BTA_GATT_INCLUDED == TRUE
#define BTA_ALL_SERVICE_MASK        0x7FFFFFFF  /* All services supported by BTA. */
#else
#define BTA_ALL_SERVICE_MASK        0x3FFFFFFF  /* All services supported by BTA. */
#endif

/*
 * Service Mask
 * Service Mask is used when several services Id are needed (e.g. Services search)
 */
#define BSA_RES_SERVICE_MASK        BTA_RES_SERVICE_MASK         /* Reserved */
#define BSA_SPP_SERVICE_MASK        BTA_SPP_SERVICE_MASK         /* Serial port profile. */
#define BSA_DUN_SERVICE_MASK        BTA_DUN_SERVICE_MASK         /* Dial-up networking profile. */
#define BSA_A2DP_SRC_SERVICE_MASK   BTA_A2DP_SRC_SERVICE_MASK    /* A2DP Source */
#define BSA_AVRCP_TG_SERVICE_MASK   BTA_AVRCP_TG_SERVICE_MASK    /* AVRCP Target */
#define BSA_HSP_SERVICE_MASK        BTA_HSP_SERVICE_MASK         /* Headset profile. */
#define BSA_HFP_SERVICE_MASK        BTA_HFP_SERVICE_MASK         /* Hands-free profile. */
#define BSA_OPP_SERVICE_MASK        BTA_OPP_SERVICE_MASK         /* Object push  */
#define BSA_FTP_SERVICE_MASK        BTA_FTP_SERVICE_MASK         /* File transfer */
#define BSA_CTP_SERVICE_MASK        BTA_CTP_SERVICE_MASK         /* Cordless Terminal */
#define BSA_ICP_SERVICE_MASK        BTA_ICP_SERVICE_MASK         /* Intercom Terminal */
#define BSA_SYNC_SERVICE_MASK       BTA_SYNC_SERVICE_MASK        /* Synchronization */
#define BSA_BPP_SERVICE_MASK        BTA_BPP_SERVICE_MASK         /* Print server */
#define BSA_BIP_SERVICE_MASK        BTA_BIP_SERVICE_MASK         /* Basic Imaging */
#define BSA_PANU_SERVICE_MASK       BTA_PANU_SERVICE_MASK        /* PAN User */
#define BSA_NAP_SERVICE_MASK        BTA_NAP_SERVICE_MASK         /* PAN Network access point */
#define BSA_GN_SERVICE_MASK         BTA_GN_SERVICE_MASK          /* PAN Group Ad-hoc networks */
#define BSA_SAP_SERVICE_MASK        BTA_SAP_SERVICE_MASK         /* PAN Group Ad-hoc networks */
#define BSA_A2DP_SERVICE_MASK       BTA_A2DP_SERVICE_MASK        /* Advanced audio distribution */
#define BSA_AVRCP_SERVICE_MASK      BTA_AVRCP_SERVICE_MASK       /* A/V remote control */
#define BSA_HID_SERVICE_MASK        BTA_HID_SERVICE_MASK         /* HID */
#define BSA_HID_DEVICE_SERVICE_MASK BTA_HID_DEVICE_SERVICE_MASK  /* HID Device */
#define BSA_VDP_SERVICE_MASK        BTA_VDP_SERVICE_MASK         /* Video distribution */
#define BSA_PBAP_SERVICE_MASK       BTA_PBAP_SERVICE_MASK        /* Phone Book */
#define BSA_HSP_HS_SERVICE_MASK     BTA_HSP_HS_SERVICE_MASK      /* HFP HS role */
#define BSA_HFP_HS_SERVICE_MASK     BTA_HFP_HS_SERVICE_MASK      /* HSP HS role */
#define BSA_MAS_SERVICE_MASK        BTA_MAS_SERVICE_MASK         /* Message Access Profile */
#define BSA_MN_SERVICE_MASK         BTA_MN_SERVICE_MASK          /* Message Notification Profile */
#define BSA_HL_SERVICE_MASK         BTA_HL_SERVICE_MASK          /* Health Profile Profile */
#define BSA_BLE_SERVICE_MASK        BTA_BLE_SERVICE_MASK         /* GATT based service */

#define BSA_ALL_SERVICE_MASK        (BTA_ALL_SERVICE_MASK & ~BTA_RES_SERVICE_MASK)  /* All services supported by BSA. */
/*
 * Service ID
 *
 * NOTES: When you add a new Service ID for BTA AND require to change the value of BTA_MAX_SERVICE_ID,
 *        make sure that the correct security ID of the new service from Security service definitions (btm_api.h)
 *        should be added to bta_service_id_to_btm_srv_id_lkup_tbl table in bta_dm_act.c.
 */

#define BTA_RES_SERVICE_ID         0           /* Reserved */
#define BTA_SPP_SERVICE_ID         1           /* Serial port profile. */
#define BTA_DUN_SERVICE_ID         2           /* Dial-up networking profile. */
#define BTA_A2DP_SRC_SERVICE_ID    3           /* A2DP Source. */ /* BSA_SPECIFIC */
#define BTA_AVRCP_TG_SERVICE_ID    4           /* AVRCP Target */ /* BSA_SPECIFIC */
#define BTA_HSP_SERVICE_ID         5           /* Headset profile. */
#define BTA_HFP_SERVICE_ID         6           /* Hands-free profile. */
#define BTA_OPP_SERVICE_ID         7           /* Object push  */
#define BTA_FTP_SERVICE_ID         8           /* File transfer */
#define BTA_CTP_SERVICE_ID         9           /* Cordless Terminal */
#define BTA_ICP_SERVICE_ID         10          /* Intercom Terminal */
#define BTA_SYNC_SERVICE_ID        11          /* Synchronization */
#define BTA_BPP_SERVICE_ID         12          /* Basic printing profile */
#define BTA_BIP_SERVICE_ID         13          /* Basic Imaging profile */
#define BTA_PANU_SERVICE_ID        14          /* PAN User */
#define BTA_NAP_SERVICE_ID         15          /* PAN Network access point */
#define BTA_GN_SERVICE_ID          16          /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_ID         17          /* SIM Access profile */
#define BTA_A2DP_SERVICE_ID        18          /* Advanced audio distribution */
#define BTA_AVRCP_SERVICE_ID       19          /* A/V remote control */
#define BTA_HID_SERVICE_ID         20          /* HID */
#define BTA_VDP_SERVICE_ID         21          /* Video distribution */
#define BTA_PBAP_SERVICE_ID        22          /* PhoneBook Access Server*/
#define BTA_HSP_HS_SERVICE_ID      23          /* HFP HS role */
#define BTA_HFP_HS_SERVICE_ID      24          /* HSP HS role */
#define BTA_MAP_SERVICE_ID         25          /* Message Access Profile */
#define BTA_MN_SERVICE_ID          26          /* Message Notification Service */
#define BTA_HDP_SERVICE_ID         27          /* Health Device Profile */
#define BTA_PCE_SERVICE_ID         28          /* PhoneBook Access Client*/
#define BTA_3DS_SERVICE_ID         29          /* 3D Sync Client */
#define BTA_HID_DEVICE_SERVICE_ID  30          /* HID Device */ /* BSA_SPECIFIC */

#if BLE_INCLUDED == TRUE && BTA_GATT_INCLUDED == TRUE
/* BLE profile service ID */
#define BTA_BLE_SERVICE_ID      31          /* GATT profile */ /* BSA_SPECIFIC */

#define BTA_MAX_SERVICE_ID      32
#else
#define BTA_MAX_SERVICE_ID      31
#endif

/*
 * Service ID
 * Service ID is used when only one service Id is needed (e.g. Service Authorization)
 */
#define BSA_RES_SERVICE_ID          BTA_RES_SERVICE_ID         /* Reserved */
#define BSA_SPP_SERVICE_ID          BTA_SPP_SERVICE_ID         /* Serial port profile. */
#define BSA_DUN_SERVICE_ID          BTA_DUN_SERVICE_ID         /* Dial-up networking profile. */
#define BSA_A2DP_SRC_SERVICE_ID     BTA_A2DP_SRC_SERVICE_ID    /* A2DP Source */
#define BSA_AVRCP_TG_SERVICE_ID     BTA_AVRCP_TG_SERVICE_ID    /* AVRCP Target */
#define BSA_HSP_SERVICE_ID          BTA_HSP_SERVICE_ID         /* Headset profile. */
#define BSA_HFP_SERVICE_ID          BTA_HFP_SERVICE_ID         /* Hands-free profile. */
#define BSA_OPP_SERVICE_ID          BTA_OPP_SERVICE_ID         /* Object push  */
#define BSA_FTP_SERVICE_ID          BTA_FTP_SERVICE_ID         /* File transfer */
#define BSA_CTP_SERVICE_ID          BTA_CTP_SERVICE_ID         /* Cordless Terminal */
#define BSA_ICP_SERVICE_ID          BTA_ICP_SERVICE_ID         /* Intercom Terminal */
#define BSA_SYNC_SERVICE_ID         BTA_SYNC_SERVICE_ID        /* Synchronization */
#define BSA_BPP_SERVICE_ID          BTA_BPP_SERVICE_ID         /* Basic printing profile */
#define BSA_BIP_SERVICE_ID          BTA_BIP_SERVICE_ID         /* Basic Imaging profile */
#define BSA_PANU_SERVICE_ID         BTA_PANU_SERVICE_ID        /* PAN User */
#define BSA_NAP_SERVICE_ID          BTA_NAP_SERVICE_ID         /* PAN Network access point */
#define BSA_GN_SERVICE_ID           BTA_GN_SERVICE_ID          /* PAN Group Ad-hoc networks */
#define BSA_SAP_SERVICE_ID          BTA_SAP_SERVICE_ID         /* SIM Access profile */
#define BSA_A2DP_SERVICE_ID         BTA_A2DP_SERVICE_ID        /* Advanced audio distribution */
#define BSA_AVRCP_SERVICE_ID        BTA_AVRCP_SERVICE_ID       /* A/V remote control */
#define BSA_HID_SERVICE_ID          BTA_HID_SERVICE_ID         /* HID */
#define BSA_HID_DEVICE_SERVICE_ID   BTA_HID_DEVICE_SERVICE_ID  /* HID Device */
#define BSA_VDP_SERVICE_ID          BTA_VDP_SERVICE_ID         /* Video distribution */
#define BSA_PBAP_SERVICE_ID         BTA_PBAP_SERVICE_ID        /* PhoneBook Access */
#define BSA_HFP_HS_SERVICE_ID       BTA_HFP_HS_SERVICE_ID      /* HFP HS role */
#define BSA_HSP_HS_SERVICE_ID       BTA_HSP_HS_SERVICE_ID      /* HSP HS role */
#define BSA_MAP_SERVICE_ID          BTA_MAP_SERVICE_ID         /* Message Access Profile */
#define BSA_HL_SERVICE_ID           BTA_HDP_SERVICE_ID         /* Health profile */
#if (defined(BLE_INCLUDED) && BLE_INCLUDED == TRUE)
#define BSA_BLE_SERVICE_ID          BTA_BLE_SERVICE_ID         /* BLE service */
#endif
#define BSA_MAX_SERVICE_ID          BTA_MAX_SERVICE_ID

/* State flag for Passthrough commands
*/
#define AVRC_STATE_PRESS    0
#define AVRC_STATE_RELEASE  1

/* Operation ID list for Passthrough commands
*/
#define AVRC_ID_SELECT      0x00    /* select */
#define AVRC_ID_UP          0x01    /* up */
#define AVRC_ID_DOWN        0x02    /* down */
#define AVRC_ID_LEFT        0x03    /* left */
#define AVRC_ID_RIGHT       0x04    /* right */
#define AVRC_ID_RIGHT_UP    0x05    /* right-up */
#define AVRC_ID_RIGHT_DOWN  0x06    /* right-down */
#define AVRC_ID_LEFT_UP     0x07    /* left-up */
#define AVRC_ID_LEFT_DOWN   0x08    /* left-down */
#define AVRC_ID_ROOT_MENU   0x09    /* root menu */
#define AVRC_ID_SETUP_MENU  0x0A    /* setup menu */
#define AVRC_ID_CONT_MENU   0x0B    /* contents menu */
#define AVRC_ID_FAV_MENU    0x0C    /* favorite menu */
#define AVRC_ID_EXIT        0x0D    /* exit */
#define AVRC_ID_0           0x20    /* 0 */
#define AVRC_ID_1           0x21    /* 1 */
#define AVRC_ID_2           0x22    /* 2 */
#define AVRC_ID_3           0x23    /* 3 */
#define AVRC_ID_4           0x24    /* 4 */
#define AVRC_ID_5           0x25    /* 5 */
#define AVRC_ID_6           0x26    /* 6 */
#define AVRC_ID_7           0x27    /* 7 */
#define AVRC_ID_8           0x28    /* 8 */
#define AVRC_ID_9           0x29    /* 9 */
#define AVRC_ID_DOT         0x2A    /* dot */
#define AVRC_ID_ENTER       0x2B    /* enter */
#define AVRC_ID_CLEAR       0x2C    /* clear */
#define AVRC_ID_CHAN_UP     0x30    /* channel up */
#define AVRC_ID_CHAN_DOWN   0x31    /* channel down */
#define AVRC_ID_PREV_CHAN   0x32    /* previous channel */
#define AVRC_ID_SOUND_SEL   0x33    /* sound select */
#define AVRC_ID_INPUT_SEL   0x34    /* input select */
#define AVRC_ID_DISP_INFO   0x35    /* display information */
#define AVRC_ID_HELP        0x36    /* help */
#define AVRC_ID_PAGE_UP     0x37    /* page up */
#define AVRC_ID_PAGE_DOWN   0x38    /* page down */
#define AVRC_ID_POWER       0x40    /* power */
#define AVRC_ID_VOL_UP      0x41    /* volume up */
#define AVRC_ID_VOL_DOWN    0x42    /* volume down */
#define AVRC_ID_MUTE        0x43    /* mute */
#define AVRC_ID_PLAY        0x44    /* play */
#define AVRC_ID_STOP        0x45    /* stop */
#define AVRC_ID_PAUSE       0x46    /* pause */
#define AVRC_ID_RECORD      0x47    /* record */
#define AVRC_ID_REWIND      0x48    /* rewind */
#define AVRC_ID_FAST_FOR    0x49    /* fast forward */
#define AVRC_ID_EJECT       0x4A    /* eject */
#define AVRC_ID_FORWARD     0x4B    /* forward */
#define AVRC_ID_BACKWARD    0x4C    /* backward */
#define AVRC_ID_ANGLE       0x50    /* angle */
#define AVRC_ID_SUBPICT     0x51    /* subpicture */
#define AVRC_ID_F1          0x71    /* F1 */
#define AVRC_ID_F2          0x72    /* F2 */
#define AVRC_ID_F3          0x73    /* F3 */
#define AVRC_ID_F4          0x74    /* F4 */
#define AVRC_ID_F5          0x75    /* F5 */
#define AVRC_ID_VENDOR      0x7E    /* vendor unique */
#define AVRC_KEYPRESSED_RELEASE 0x80

/* operation id list for BSA_AvRemoteCmd */
#define BSA_AV_RC_SELECT      AVRC_ID_SELECT      /* select */
#define BSA_AV_RC_UP          AVRC_ID_UP          /* up */
#define BSA_AV_RC_DOWN        AVRC_ID_DOWN        /* down */
#define BSA_AV_RC_LEFT        AVRC_ID_LEFT        /* left */
#define BSA_AV_RC_RIGHT       AVRC_ID_RIGHT       /* right */
#define BSA_AV_RC_RIGHT_UP    AVRC_ID_RIGHT_UP    /* right-up */
#define BSA_AV_RC_RIGHT_DOWN  AVRC_ID_RIGHT_DOWN  /* right-down */
#define BSA_AV_RC_LEFT_UP     AVRC_ID_LEFT_UP     /* left-up */
#define BSA_AV_RC_LEFT_DOWN   AVRC_ID_LEFT_DOWN   /* left-down */
#define BSA_AV_RC_ROOT_MENU   AVRC_ID_ROOT_MENU   /* root menu */
#define BSA_AV_RC_SETUP_MENU  AVRC_ID_SETUP_MENU  /* setup menu */
#define BSA_AV_RC_CONT_MENU   AVRC_ID_CONT_MENU   /* contents menu */
#define BSA_AV_RC_FAV_MENU    AVRC_ID_FAV_MENU    /* favorite menu */
#define BSA_AV_RC_EXIT        AVRC_ID_EXIT        /* exit */
#define BSA_AV_RC_0           AVRC_ID_0           /* 0 */
#define BSA_AV_RC_1           AVRC_ID_1           /* 1 */
#define BSA_AV_RC_2           AVRC_ID_2           /* 2 */
#define BSA_AV_RC_3           AVRC_ID_3           /* 3 */
#define BSA_AV_RC_4           AVRC_ID_4           /* 4 */
#define BSA_AV_RC_5           AVRC_ID_5           /* 5 */
#define BSA_AV_RC_6           AVRC_ID_6           /* 6 */
#define BSA_AV_RC_7           AVRC_ID_7           /* 7 */
#define BSA_AV_RC_8           AVRC_ID_8           /* 8 */
#define BSA_AV_RC_9           AVRC_ID_9           /* 9 */
#define BSA_AV_RC_DOT         AVRC_ID_DOT         /* dot */
#define BSA_AV_RC_ENTER       AVRC_ID_ENTER       /* enter */
#define BSA_AV_RC_CLEAR       AVRC_ID_CLEAR       /* clear */
#define BSA_AV_RC_CHAN_UP     AVRC_ID_CHAN_UP     /* channel up */
#define BSA_AV_RC_CHAN_DOWN   AVRC_ID_CHAN_DOWN   /* channel down */
#define BSA_AV_RC_PREV_CHAN   AVRC_ID_PREV_CHAN   /* previous channel */
#define BSA_AV_RC_SOUND_SEL   AVRC_ID_SOUND_SEL   /* sound select */
#define BSA_AV_RC_INPUT_SEL   AVRC_ID_INPUT_SEL   /* input select */
#define BSA_AV_RC_DISP_INFO   AVRC_ID_DISP_INFO   /* display information */
#define BSA_AV_RC_HELP        AVRC_ID_HELP        /* help */
#define BSA_AV_RC_PAGE_UP     AVRC_ID_PAGE_UP     /* page up */
#define BSA_AV_RC_PAGE_DOWN   AVRC_ID_PAGE_DOWN   /* page down */
#define BSA_AV_RC_POWER       AVRC_ID_POWER       /* power */
#define BSA_AV_RC_VOL_UP      AVRC_ID_VOL_UP      /* volume up */
#define BSA_AV_RC_VOL_DOWN    AVRC_ID_VOL_DOWN    /* volume down */
#define BSA_AV_RC_MUTE        AVRC_ID_MUTE        /* mute */
#define BSA_AV_RC_PLAY        AVRC_ID_PLAY        /* play */
#define BSA_AV_RC_STOP        AVRC_ID_STOP        /* stop */
#define BSA_AV_RC_PAUSE       AVRC_ID_PAUSE       /* pause */
#define BSA_AV_RC_RECORD      AVRC_ID_RECORD      /* record */
#define BSA_AV_RC_REWIND      AVRC_ID_REWIND      /* rewind */
#define BSA_AV_RC_FAST_FOR    AVRC_ID_FAST_FOR    /* fast forward */
#define BSA_AV_RC_EJECT       AVRC_ID_EJECT       /* eject */
#define BSA_AV_RC_FORWARD     AVRC_ID_FORWARD     /* forward */
#define BSA_AV_RC_BACKWARD    AVRC_ID_BACKWARD    /* backward */
#define BSA_AV_RC_ANGLE       AVRC_ID_ANGLE       /* angle */
#define BSA_AV_RC_SUBPICT     AVRC_ID_SUBPICT     /* subpicture */
#define BSA_AV_RC_F1          AVRC_ID_F1          /* F1 */
#define BSA_AV_RC_F2          AVRC_ID_F2          /* F2 */
#define BSA_AV_RC_F3          AVRC_ID_F3          /* F3 */
#define BSA_AV_RC_F4          AVRC_ID_F4          /* F4 */
#define BSA_AV_RC_F5          AVRC_ID_F5          /* F5 */
#define BSA_AV_RC_VENDOR      AVRC_ID_VENDOR      /* vendor unique */

typedef struct {
	char *ops_push_dir; /* opc push file to ops dir */
	char *ops_pull_file; /* opc pull file from ops_file */
} ops_accept_param;

typedef struct {
	int select_num; /* 0 Device from XML database (already paired)
			   1 Device found in last discovery */
	int device_index;
	char *file_path; /* opc push file_path to ops */
} opc_push_param;

typedef struct {
	int select_num; /* 0 Device from XML database (already paired)
			   1 Device found in last discovery */
	int device_index;
} av_open_param;

typedef enum {
	/* means not currently in call set up */
	CALLSETUP_STATE_NO_CALL_SETUP = 0,
	/* means an incoming call process ongoing */
	CALLSETUP_STATE_INCOMMING_CALL,
	/* means an outgoing call set up is ongoing */
	CALLSETUP_STATE_OUTGOING_CALL,
	/* means remote party being alerted in an outgoing call */
	CALLSETUP_STATE_REMOTE_BEING_ALERTED_IN_OUTGOING_CALL,
	/* means a call is waiting */
	CALLSETUP_STATE_WAITING_CALL,

	/* means there are no calls in progress */
	CALL_STATE_NO_CALLS_ONGOING,
	/* means at least one call is in progress */
	CALL_STATE_LEAST_ONE_CALL_ONGOING,

	/* No calls held */
	CALLHELD_STATE_NO_CALL_HELD,
	/* Call is placed on hold or active/held calls swapped */
	CALLHELD_STATE_PLACED_ON_HOLD_OR_SWAPPED,
	/* Call on hold, no active call */
	CALLHELD_STATE_CALL_ON_HOLD_AND_NO_ACTIVE_CALL

} BSA_CALL_STATE;

typedef enum {
	BT_LINK_DISCONNECTING,
	BT_LINK_DISCONNECTED,
	BT_LINK_CONNECTING,
	BT_LINK_CONNECTED,
	BT_LINK_CONNECT_FAILED,
} bsa_link_status;

typedef enum {
	BSA_APP_RECONNECT,
	BSA_APP_DISCOVERY
} BSA_OPEN_TYPE;

typedef enum {
	BTHF_CHLD_TYPE_RELEASEHELD,
	BTHF_CHLD_TYPE_RELEASEACTIVE_ACCEPTHELD,
	BTHF_CHLD_TYPE_HOLDACTIVE_ACCEPTHELD,
	BTHF_CHLD_TYPE_ADDHELDTOCONF
} tBSA_BTHF_CHLD_TYPE_T;

enum {
	USE_HS_AVK = 0,
	USE_HS_ONLY,
	USE_AVK_ONLY
};

typedef enum {
	BTHF_VOLUME_TYPE_SPK = 0,	/* Update speaker volume */
	BTHF_VOLUME_TYPE_MIC = 1	/* Update microphone volume */
} tBSA_BTHF_VOLUME_TYPE_T;

/* Discovery callback events */
typedef enum
{
	BSA_DISC_NEW_EVT, 		/* a New Device has been discovered */
	BSA_DISC_CMPL_EVT, 		/* End of Discovery */
	BSA_DISC_DEV_INFO_EVT, 		/* Device Info discovery event */
	BSA_DISC_REMOTE_NAME_EVT  	/* Read remote device name event */
} tBSA_DISC_EVT;

/* Vendor and Product Identification
 * of the peer device
 * */
typedef struct
{
	BOOLEAN valid; 			/* TRUE if this entry is valid */
	UINT16 vendor_id_source; 	/* Indicate if the vendor field is BT or USB */
	UINT16 vendor; 			/* Vendor Id of the peer device */
	UINT16 product; 		/* Product Id of the peer device */
	UINT16 version; 		/* Version of the peer device */
} tBSA_DISC_VID_PID;

typedef struct
{
	BD_ADDR bd_addr; 		/* BD address peer device. */
	DEV_CLASS class_of_device; 	/* Class of Device */
	BD_NAME name; 			/* Name of peer device. */
	int rssi; 			/* The rssi value */
	tBSA_SERVICE_MASK services; 	/* Service discovery discovered */
	tBSA_DISC_VID_PID eir_vid_pid[BSA_DISC_VID_PID_MAX];
	UINT8 eir_data[BSA_EIR_DATA_LENGTH];  /* Full EIR data */
	UINT8 inq_result_type;
	UINT8 ble_addr_type;
	tBT_DEVICE_TYPE device_type;
} tBSA_DISC_REMOTE_DEV;

typedef struct
{
	tBSA_STATUS status;		/* Status of the request */
	BD_ADDR bd_addr;		/* BD address peer device. */
	UINT8 index;
	UINT16 spec_id;
	BOOLEAN primary;
	UINT16 vendor;
	UINT16 vendor_id_source;
	UINT16 product;
	UINT16 version;
} tBSA_DISC_DEV_INFO_MSG;

typedef struct
{
	UINT16      status;
	BD_ADDR     bd_addr;
	UINT16      length;
	BD_NAME     remote_bd_name;
} tBSA_DISC_READ_REMOTE_NAME_MSG;

/* Structure associated with BSA_DISC_NEW_EVT */
typedef tBSA_DISC_REMOTE_DEV tBSA_DISC_NEW_MSG;

/* Union of all Discovery callback structures */
typedef union
{
	tBSA_DISC_NEW_MSG disc_new;			/* a New Device has been discovered */
	tBSA_DISC_DEV_INFO_MSG dev_info;		/* Device Info of a device */
	tBSA_DISC_READ_REMOTE_NAME_MSG remote_name; 	/* Name of Remote device */
} tBSA_DISC_MSG;

typedef struct
{
	BOOLEAN in_use; /* TRUE is this element is used, FALSE otherwise */
	tBSA_DISC_REMOTE_DEV device; /* Device Info */
} tBSA_DISC_DEV;

typedef struct
{
	tBSA_DISC_DEV devs[APP_DISC_NB_DEVICES];
} tAPP_DISCOVERY_CB;

/* data for meta data items */
#define BSA_AVK_ATTR_STR_LEN_MAX 102

/* data for get element attribute response */
#define BSA_AVK_ELEMENT_ATTR_MAX 7

typedef UINT8 tAVRC_STS;

/* media string */
typedef struct
{
	UINT8       data[BSA_AVK_ATTR_STR_LEN_MAX];
	UINT16      charset_id;
	UINT16      str_len;
} tBSA_AVK_STRING;

/* attibute entry */
typedef struct
{
	UINT32             attr_id;
	tBSA_AVK_STRING    name;
} tBSA_AVK_ATTR_ENTRY;

typedef struct
{
	tAVRC_STS               status;
	UINT8                   num_attr;
	tBSA_AVK_ATTR_ENTRY     attr_entry[BSA_AVK_ELEMENT_ATTR_MAX];
} tBSA_AVK_GET_ELEMENT_ATTR_MSG;

/* GetPlayStatus */
typedef struct
{
	UINT8       pdu;
	tAVRC_STS   status;
	UINT8       opcode;         /* Op Code (copied from avrc_cmd.opcode by AVRC_BldResponse user. invalid one to generate according to pdu) */
	UINT32      song_len;
	UINT32      song_pos;
	UINT8       play_status;
} tAVRC_GET_PLAY_STATUS_RSP;

typedef tAVRC_GET_PLAY_STATUS_RSP   tBSA_AVK_GET_PLAY_STATUS_MSG;

typedef void (tBSA_DISC_CBACK)(tBSA_DISC_EVT event, tBSA_DISC_MSG *p_data);

typedef struct {
	char	*in_buffer;
	char	*out_buffer;
	UINT32	in_len;
	UINT32	out_len;
	UINT32 	recv_rate;
	UINT32 	recv_channel;
	UINT32 	recv_bits;
	UINT32 	cfg_rate;
	UINT32	cfg_channel;
	UINT32 	cfg_bits;
} avk_data_cback_msg;

typedef UINT8 tBSA_AVK_EVT;
typedef void (*avk_data_cback_t)(avk_data_cback_msg *avk_msg);
/* AV callback */
typedef void (*avk_evt_cback_t)(tBSA_AVK_EVT event, void *p_data);

typedef struct {
	avk_evt_cback_t  mozart_avk_evt_cback;
	avk_data_cback_t mozart_avk_data_cback;
} avk_cback_init_data;

typedef struct bt_manager_init_info
{
	char *bt_name;
	char *bt_ble_name;
	int discoverable;
	int connectable;
	unsigned char out_bd_addr[BD_ADDR_LEN];
} bt_init_info;

typedef struct bsa_ble_create_service_data {
	int	server_num;	/* server number which is distributed */
	int	attr_num;
	UINT16	service_uuid;	/*service uuid */
	UINT16  num_handle;
	BOOLEAN	is_primary;
} ble_create_service_data;

typedef struct bsa_ble_server_indication {
	int conn_id;
	int server_num;
	int attr_num;
	int length_of_data;
	UINT8 *value;
	BOOLEAN need_confirm;	 /* TRUE: Indicate, FALSE: Notify */
} ble_server_indication;

typedef struct bsa_ble_start_service_data {
	int server_num;
	int attr_num;
} ble_start_service_data;

typedef struct bsa_ble_add_character_data {
	int server_num;
	int srvc_attr_num;
	int char_attr_num;
	int is_descript;
	int attribute_permission;
	int characteristic_property;
	UINT16	char_uuid;
} ble_char_data;

#define NOTIFY_CONN_MAX_NUM  20
typedef struct bsa_ble_character_notify_data {
	ble_char_data char_data;
	int conn_id[NOTIFY_CONN_MAX_NUM];
} ble_char_notify;

typedef struct {
	UINT16 service_uuid;	/* Enter Service UUID to read(eg. x1800) */
	UINT16 char_uuid;	/* Enter Char UUID to read(eg. x2a00) */
	UINT16 descr_id;	/* Enter Descriptor type UUID to read(eg. x2902) */
	int client_num;
	int is_descript;	/* Select descriptor? (yes=1 or no=0),default 0 */
	int ser_inst_id;	/* Enter Instance ID for Service UUID(eg. 0,1,2..), default 0 */
	int char_inst_id;	/* Enter Instance ID for Char UUID(eg. 0,1,2..) */
	int is_primary;		/* Enter Is_primary value(eg:0,1) */
} ble_client_read_data;

typedef struct {
	UINT16 service_uuid;	/* Service UUID to write */
	UINT16 char_uuid;	/* Char UUID to write */
	UINT16 descr_id;	/* Descriptor type UUID to write(eg. x2902) */
	UINT8 desc_value;	/* Descriptor value to write(eg. x01) */
	UINT8 write_data[BSA_BLE_CL_WRITE_MAX];		/* write data */
	UINT16 write_len;	/* write length: bytes */
	UINT8 write_type;	/* 1-GATT_WRITE_NO_RSP 2-GATT_WRITE */
	int client_num;
	int is_descript;	/* select descriptor? (yes=1 or no=0) */
	int ser_inst_id;	/* Instance ID for Service UUID, default 0 */
	int char_inst_id;	/* Instance ID for Char UUID(eg. 0,1,2..) */
	int is_primary;		/* Is_primary value(eg:0,1) */
} ble_client_write_data;

typedef struct {
	int device_index;	/* 0 Device from XML database (already paired), 1 Device found in last discovery */
	char *ble_name; 	/* The ble device you want to connect */
	int client_num;
	int direct;		/* Direct connection:1, Background connection:0 */
	BD_ADDR bd_addr;
} ble_client_connect_data;

typedef struct {
	int client_num;
	int service_id;
	int character_id;
	int ser_inst_id;
	int char_inst_id;
	int is_primary;
} BLE_CL_NOTIFREG;

typedef struct
{
	tBT_UUID       attr_UUID;
	UINT16         service_id;
	UINT16         attr_id;
	UINT8          attr_type;
	UINT8          prop;
	BOOLEAN        is_pri;
	BOOLEAN        wait_flag;
	UINT8          value[BSA_BLE_MAX_ATTR_LEN];
	UINT16         value_len;
} tAPP_BLE_ATTRIBUTE;

typedef struct
{
	tBT_UUID        service_UUID;
	BOOLEAN         enabled;
	tBSA_BLE_IF     client_if;
	UINT16          conn_id;
	BD_ADDR         server_addr;
	BOOLEAN         write_pending;
	BOOLEAN         read_pending;
} tAPP_BLE_CLIENT;

typedef struct
{
	BOOLEAN             enabled;
	tBSA_BLE_IF         server_if;
	UINT16              conn_id;
	tAPP_BLE_ATTRIBUTE  attr[BSA_BLE_ATTRIBUTE_MAX];
} tAPP_BLE_SERVER;

typedef struct
{
	tAPP_BLE_CLIENT ble_client[BSA_BLE_CLIENT_MAX];
	tAPP_BLE_SERVER ble_server[BSA_BLE_SERVER_MAX];
} tAPP_BLE_CB;

typedef enum
{
	/* BLE Client events */
	BSA_BLE_CL_DEREGISTER_EVT,    /* BLE client is registered. */
	BSA_BLE_CL_OPEN_EVT,          /* BLE open request status  event */
	BSA_BLE_CL_CANCEL_OPEN_EVT,   /* BLE cancel open request status event */
	BSA_BLE_CL_READ_EVT,          /* BLE read characteristic/descriptor event */
	BSA_BLE_CL_WRITE_EVT,         /* BLE write characteristic/descriptor event */
	BSA_BLE_CL_CLOSE_EVT,         /* GATTC  close request status event */
	BSA_BLE_CL_SEARCH_CMPL_EVT,   /* GATT discovery complete event */
	BSA_BLE_CL_SEARCH_RES_EVT,    /* GATT discovery result event */
	BSA_BLE_CL_NOTIF_EVT,         /* GATT attribute notification event */
	BSA_BLE_CL_CONGEST_EVT,       /* GATT congestion/uncongestion event */
	BSA_BLE_CL_CACHE_SAVE_EVT,
	BSA_BLE_CL_CACHE_LOAD_EVT,

	/* BLE Server events */
	BSA_BLE_SE_DEREGISTER_EVT,    /* BLE Server is deregistered */
	BSA_BLE_SE_CREATE_EVT,        /* Service is created */
	BSA_BLE_SE_ADDCHAR_EVT,       /* char data is added */
	BSA_BLE_SE_START_EVT,         /* Service is started */
	BSA_BLE_SE_WRITE_EVT,         /* Write request from client */
	BSA_BLE_SE_CONGEST_EVT,       /* Congestion event */
	BSA_BLE_SE_READ_EVT,          /* Read request from client */
	BSA_BLE_SE_EXEC_WRITE_EVT,    /* Exec write request from client */
	BSA_BLE_SE_OPEN_EVT,          /* Connect request from client */
	BSA_BLE_SE_CLOSE_EVT,         /* Disconnect request from client */
	BSA_BLE_SE_CONF_EVT,
} tBSA_BLE_EVT;

/* Client callback function events */

/* callback event data for BSA_BLE_CL_OPEN_EVT event */
typedef struct
{
	tBSA_STATUS     status; /* operation status */
	UINT16          conn_id;
	tBSA_BLE_IF     client_if; /* Client interface ID */
	BD_ADDR         bd_addr;
} tBSA_BLE_CL_OPEN_MSG;

/* callback event data for BSA_BLE_CL_CANCEL_OPEN_EVT event */
typedef struct
{
	tBSA_STATUS     status; /* operation status */
} tBSA_BLE_CL_CANCEL_OPEN_MSG;

/* callback event data for BSA_BLE_CL_CLOSE_EVT event */
typedef struct
{
	tBSA_STATUS         status;
	UINT16              conn_id;
	tBSA_BLE_IF         client_if;
	BD_ADDR             remote_bda;
	tBSA_BLE_REASON     reason;         /* disconnect reason code, not useful when connect event is reported */
} tBSA_BLE_CL_CLOSE_MSG;

/* callback event data for BSA_BLE_CL_DEREGISTER_EVT event */
typedef struct
{
	tBSA_STATUS    status; /* operation status */
	tBSA_BLE_IF    client_if; /* Client interface ID */
} tBSA_BLE_CL_DEREGISTER_MSG;

/* callback event data for BSA_BLE_CL_SEARCH_RES_EVT event */
typedef struct
{
	UINT16       conn_id;
	tBTA_GATT_SRVC_ID  service_uuid;
} tBSA_BLE_CL_SEARCH_RES_MSG;

/* callback event data for BSA_BLE_CL_SEARCH_CMPL_EVT event */
typedef struct
{
	tBSA_STATUS    status; /* operation status */
	UINT16         conn_id;
} tBSA_BLE_CL_SEARCH_CMPL_MSG;

#define MAX_READ_LEN 100
/* callback event data for BSA_BLE_CL_READ_EVT event */
typedef struct
{
	tBSA_STATUS         status;
	UINT16              conn_id;
	tBTA_GATT_SRVC_ID         srvc_id;
	tBSA_BLE_ID         char_id;
	tBTA_GATT_ID            descr_type;
	UINT16              len;
	UINT8               value[MAX_READ_LEN];
} tBSA_BLE_CL_READ_MSG;

/* callback event data for BSA_BLE_CL_WRITE_EVT event */
typedef struct
{
	UINT16              conn_id;
	tBSA_STATUS         status;
	tBTA_GATT_SRVC_ID         srvc_id;
	tBSA_BLE_ID         char_id;
	tBTA_GATT_ID            descr_type;
} tBSA_BLE_CL_WRITE_MSG;

/* GATT write type enumeration */
enum
{
    GATT_WRITE_NO_RSP = 1,
    GATT_WRITE ,
    GATT_WRITE_PREPARE
};
#define BTA_GATTC_TYPE_WRITE             GATT_WRITE
#define BTA_GATTC_TYPE_WRITE_NO_RSP      GATT_WRITE_NO_RSP

/* callback event data for BSA_BLE_CL_NOTIF_EVT event */
typedef struct
{
	UINT16              conn_id;
	BD_ADDR             bda;
	tBTA_GATTC_CHAR_ID  char_id;
	tBTA_GATT_ID            descr_type;
	UINT16              len;
	UINT8               value[BSA_BLE_MAX_ATTR_LEN];
	BOOLEAN             is_notify;
} tBSA_BLE_CL_NOTIF_MSG;

typedef struct
{
	UINT16              conn_id;
	BOOLEAN             congested; /* congestion indicator */
}tBSA_BLE_CL_CONGEST_MSG;

typedef struct
{
    tBT_UUID    uuid;
    UINT16      s_handle;
    UINT16      e_handle;   /* used for service only */
    UINT8       attr_type;
    UINT8       id;
    UINT8       prop;       /* used when attribute type is characteristic */
    BOOLEAN     is_primary; /* used when attribute type is service */
} tBTA_GATTC_NV_ATTR;

typedef UINT8 tBTA_GATT_STATUS;
typedef UINT8   tGATT_EXEC_FLAG;
typedef tGATT_EXEC_FLAG tBTA_GATT_EXEC_FLAG;
#define BTA_GATTC_NV_LOAD_MAX   10
#define BSA_BLE_CL_NV_LOAD_MAX   BTA_GATTC_NV_LOAD_MAX
/* callback event data for BSA_BLE_CL_CACHE_SAVE_EVT event */
/* attributes in one service */
typedef struct
{
	UINT16               evt;
	UINT16               num_attr;
	UINT16               attr_index;
	UINT16               conn_id;
	BD_ADDR              bd_addr;
	tBTA_GATTC_NV_ATTR   attr[BSA_BLE_CL_NV_LOAD_MAX];
} tBSA_BLE_CL_CACHE_SAVE_MSG;

/* callback event data for BSA_BLE_CL_CACHE_LOAD_EVT event */
typedef struct
{
	UINT16         conn_id;
	tBSA_STATUS    status;
	BD_ADDR        bd_addr;
} tBSA_BLE_CL_CACHE_LOAD_MSG;


/* BSA BLE Server Host callback events */
/* Server callback function events */

/* callback event data for BSA_BLE_SE_DEREGISTER_EVT event */
typedef struct
{
	tBSA_STATUS    status; /* operation status */
	tBSA_BLE_IF    server_if; /* Server interface ID */
} tBSA_BLE_SE_DEREGISTER_MSG;

/* callback event data for BSA_BLE_SE_CREATE_EVT event */
typedef struct
{
	tBSA_BLE_IF    server_if;
	UINT16         service_id;
	tBSA_STATUS    status;
} tBSA_BLE_SE_CREATE_MSG;

/* callback event data for BSA_BLE_SE_ADDCHAR_EVT event */
typedef struct
{
	tBSA_BLE_IF    server_if;
	UINT16         service_id;
	UINT16         attr_id;
	tBSA_STATUS    status;
	BOOLEAN        is_discr;
} tBSA_BLE_SE_ADDCHAR_MSG;

/* callback event data for BSA_BLE_SE_START_EVT event */
typedef struct
{
	tBSA_BLE_IF    server_if;
	UINT16         service_id;
	tBSA_STATUS    status;
} tBSA_BLE_SE_START_MSG;

typedef struct
{
	BD_ADDR       remote_bda;
	UINT32        trans_id;
	UINT16        conn_id;
	UINT16        handle;
	UINT16        offset;
	BOOLEAN       is_long;
	tBSA_STATUS   status;
} tBSA_BLE_SE_READ_MSG;

typedef struct
{
	BD_ADDR       remote_bda;
	UINT32        trans_id;
	UINT16        conn_id;
	UINT16        handle;     /* attribute handle */
	UINT16        offset;     /* attribute value offset, if no offset is needed for the command, ignore it */
	UINT16        len;        /* length of attribute value */
	UINT8         value[BSA_BLE_MAX_ATTR_LEN];  /* the actual attribute value */
	BOOLEAN       need_rsp;   /* need write response */
	BOOLEAN       is_prep;    /* is prepare write */
	tBSA_STATUS   status;
} tBSA_BLE_SE_WRITE_MSG;

typedef struct
{
	BD_ADDR     remote_bda;
	UINT32      trans_id;
	UINT16      conn_id;
	tGATT_EXEC_FLAG     exec_write;
	tBSA_STATUS     status;
} tBSA_BLE_SE_EXEC_WRITE_MSG;

typedef struct
{
	tBSA_BLE_IF        server_if;
	BD_ADDR            remote_bda;
	UINT16             conn_id;
	tBSA_BLE_REASON    reason;
} tBSA_BLE_SE_OPEN_MSG;

typedef tBSA_BLE_SE_OPEN_MSG tBSA_BLE_SE_CLOSE_MSG;

typedef struct
{
	UINT16              conn_id;
	BOOLEAN             congested; /* congestion indicator */
}tBSA_BLE_SE_CONGEST_MSG;

typedef struct
{
	UINT16              conn_id;    /* connection ID */
	tBTA_GATT_STATUS    status;     /* notification/indication status */
} tBSA_BLE_SE_CONF_MSG;

/* Union of data associated with HD callback */
typedef union
{
	tBSA_BLE_CL_OPEN_MSG         cli_open;          /* BSA_BLE_CL_OPEN_EVT */
	tBSA_BLE_CL_CANCEL_OPEN_MSG  cli_cancel_open;   /* BSA_BLE_CL_CANCEL_OPEN_EVT */
	tBSA_BLE_CL_SEARCH_RES_MSG   cli_search_res;    /* BSA_BLE_CL_SEARCH_RES_EVT */
	tBSA_BLE_CL_SEARCH_CMPL_MSG  cli_search_cmpl;   /* BSA_BLE_CL_SEARCH_CMPL_EVT */
	tBSA_BLE_CL_READ_MSG         cli_read;          /* BSA_BLE_CL_READ_EVT */
	tBSA_BLE_CL_WRITE_MSG        cli_write;         /* BSA_BLE_CL_WRITE_EVT */
	tBSA_BLE_CL_NOTIF_MSG        cli_notif;         /* BSA_BLE_CL_NOTIF_EVT */
	tBSA_BLE_CL_CONGEST_MSG      cli_congest;       /* BSA_BLE_CL_CONGEST_EVT */
	tBSA_BLE_CL_CLOSE_MSG        cli_close;         /* BSA_BLE_CL_CLOSE_EVT */
	tBSA_BLE_CL_DEREGISTER_MSG   cli_deregister;    /* BSA_BLE_CL_DEREGISTER_EVT */
	tBSA_BLE_CL_CACHE_SAVE_MSG   cli_cache_save;    /* BSA_BLE_SE_CACHE_SAVE_EVT */
	tBSA_BLE_CL_CACHE_LOAD_MSG   cli_cache_load;    /* BSA_BLE_SE_CACHE_LOAD_EVT */

	tBSA_BLE_SE_DEREGISTER_MSG   ser_deregister;    /* BSA_BLE_SE_DEREGISTER_EVT */
	tBSA_BLE_SE_CREATE_MSG       ser_create;        /* BSA_BLE_SE_CREATE_EVT */
	tBSA_BLE_SE_ADDCHAR_MSG      ser_addchar;       /* BSA_BLE_SE_ADDCHAR_EVT */
	tBSA_BLE_SE_START_MSG        ser_start;         /* BSA_BLE_SE_START_EVT */
	tBSA_BLE_SE_READ_MSG         ser_read;          /* BSA_BLE_SE_READ_EVT */
	tBSA_BLE_SE_WRITE_MSG        ser_write;         /* BSA_BLE_SE_WRITE_EVT */
	tBSA_BLE_SE_OPEN_MSG         ser_open;          /* BSA_BLE_SE_OPEN_EVT */
	tBSA_BLE_SE_EXEC_WRITE_MSG   ser_exec_write;    /* BSA_BLE_SE_EXEC_WRITE_EVT */
	tBSA_BLE_SE_CLOSE_MSG        ser_close;         /* BSA_BLE_SE_CLOSE_EVT */
	tBSA_BLE_SE_CONGEST_MSG      ser_congest;       /* BSA_BLE_CL_CONGEST_EVT */
	tBSA_BLE_SE_CONF_MSG         ser_conf;         /* BSA_BLE_SE_CONF_EVT */

} tBSA_BLE_MSG;

typedef int (*bt_ble_func_t)(tBSA_BLE_EVT event, tBSA_BLE_MSG *p_data);

typedef struct {
	bt_ble_func_t ble_client_profile_cback;
	bt_ble_func_t ble_server_profile_cback;
} bt_ble_callback;

typedef enum {
	SEC_NONE = 0,
	SEC_AUTHENTICATION = 1,
	SEC_ENCRYPTION_AUTHENTICATION = 3,
	SEC_AUTHORIZATION = 4,
	SEC_ENCRYPTION_AUTHENTICATION_AUTHORIZATION = 7
} bt_sec_type;

typedef struct {
	int device_index;
	int disc_index;
	bt_sec_type sec_type;
	tBSA_HH_PROTO_MODE mode;
} ble_hh_connect_data;
typedef int (*bt_hh_func_t)(UINT8 *p_data, UINT16 length);

typedef UINT8 tBTM_IO_CAP;
typedef tBTM_IO_CAP     tBTA_IO_CAP;
typedef tBTA_IO_CAP tBSA_SEC_IO_CAP;

#define BD_FEATURES_LEN 8
typedef UINT8 BD_FEATURES[BD_FEATURES_LEN]; /* LMP features supported by device */

#define BT_OCTET16_LEN    16
typedef UINT8 BT_OCTET16[BT_OCTET16_LEN];   /* octet array: size 16 */

typedef UINT8 tBLE_ADDR_TYPE;
/* remote device */
typedef struct
{
	BOOLEAN in_use;
	BD_ADDR bd_addr;
	BD_NAME name;
	DEV_CLASS class_of_device;
	tBSA_SERVICE_MASK available_services;
	tBSA_SERVICE_MASK trusted_services;
	BOOLEAN is_default_hs;
	BOOLEAN stored;
	BOOLEAN link_key_present;
	LINK_KEY link_key;
	unsigned char key_type;
	tBSA_SEC_IO_CAP io_cap;
	UINT16 pid;
	UINT16 vid;
	UINT16 version;
	BD_FEATURES features;
	UINT8 lmp_version;
#if (defined(BLE_INCLUDED) && BLE_INCLUDED == TRUE)
	UINT8 ble_addr_type;
	tBT_DEVICE_TYPE device_type;
	UINT8 inq_result_type;
	BOOLEAN ble_link_key_present;
	/* KEY_PENC */
	BT_OCTET16 penc_ltk;
	BT_OCTET8 penc_rand;
	UINT16 penc_ediv;
	UINT8 penc_sec_level;
	UINT8 penc_key_size;
	/* KEY_PID */
	BT_OCTET16 pid_irk;
	tBLE_ADDR_TYPE pid_addr_type;
	BD_ADDR pid_static_addr;
	/* KEY_PCSRK */
	UINT32 pcsrk_counter;
	BT_OCTET16 pcsrk_csrk;
	UINT8 pcsrk_sec_level;
	/* KEY_LCSRK */
	UINT32 lcsrk_counter;
	UINT16 lcsrk_div;
	UINT8 lcsrk_sec_level;
	/* KEY_LENC */
	UINT16 lenc_div;
	UINT8 lenc_key_size;
	UINT8 lenc_sec_level;
#endif
} tAPP_XML_REM_DEVICE;

/*
 * Structure used to configure the AV media feeding
 */
typedef struct {
	UINT16 sampling_freq;   /* 44100, 48000 etc */
	UINT16 num_channel;     /* 1 for mono or 2 stereo */
	UINT8  bit_per_sample;  /* Number of bits per sample (8, 16) */
} tBSA_AV_MEDIA_FEED_CFG_PCM;

enum RC_COMMANDS
{
	APP_AV_IDLE,
	APP_AV_START,
	APP_AV_STOP,
	APP_AV_PAUSE,
	APP_AV_FORWARD,
	APP_AV_BACKWARD,
	APP_AV_VOL_UP,
	APP_AV_VOL_DOWN
};

/* Play states */
enum
{
	APP_AV_PLAY_STOPPED,
	APP_AV_PLAY_STARTED,
	APP_AV_PLAY_PAUSED,
	APP_AV_PLAY_STOPPING
};
#define BTA_SERVICE_NAME_LEN    35
#define BSA_SERVICE_NAME_LEN BTA_SERVICE_NAME_LEN

/* Events generated */
#define UIPC_OPEN_EVT                       0x01
#define UIPC_CLOSE_EVT                      0x02
#define UIPC_RX_DATA_EVT                    0x03
#define UIPC_RX_DATA_READY_EVT              0x04
#define UIPC_TX_DATA_READY_EVT              0x05

typedef UINT8 tBSA_SERVICE_ID;
typedef UINT8 tBTA_SEC;
typedef tBTA_SEC tBSA_SEC_AUTH;
typedef struct {
	BD_ADDR bd_addr;
	tBSA_SERVICE_ID service;
	tBSA_SEC_AUTH sec_mask;
	char service_name[BSA_SERVICE_NAME_LEN];
	tBT_UUID uuid;
} tBSA_DG_OPEN;

typedef struct {
	int connect_type;
	int device_index;
	int uuid[16];
	BOOLEAN is_128uuid;
	tBSA_DG_OPEN param;
} dg_open_paramters;

typedef struct {
	int uuid[16];
	tBSA_DG_OPEN param;
} dg_start_service_paramters;

typedef int (*bt_dg_func_t)(UINT16 event, UINT8 *p_data, UINT32 length);
/*******************************************************************************
 **
 ** Function         mozart_bluetooth_init
 **
 ** Description      Initial bluetooth function.
 **
 ** Parameters	     bt_name:       bluetooth device name
 **		     discoverable:  0 is no discovery, 1 is discoverable.
 ** 		     connectable:   0 is disconnect,   1 is connectable.
 **		     out_bd_addr:   0 is use bt controler bd_addr, other is user-defined.
 **
 ** Returns          0 is ok, -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_init(bt_init_info *bt_info);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_uninit
 **
 ** Description      Uninitial bluetooth function.
 **
 ** Returns          0 is ok, -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_uninit(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_auto_reconnect
 **
 ** Description      Auto reconnect the last connected BT device
 **
 ** Parameters       type: USE_HS_AVK is use HFP and A2DP
 **			   USE_HS_ONLY is only use HFP
 **			   USE_AVK_ONLY is only use A2DP.
 **		     select_num: Which device you want to reconnected, 0 is the last connected device
 **
 ** Returns           0 if successful
 **		     -1 is connect failed
 **		     -2 is bt_reconnect_devices.xml not existed, no connected device
 **
 *******************************************************************************/
extern int mozart_bluetooth_auto_reconnect(int type, unsigned int select_num);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_disconnect
 **
 ** Description      Bluetooth disconnect connection with bt device
 **
 ** Parameters       type: USE_HS_AVK is use HFP and A2DP, USE_HS_ONLY is only use HFP, USE_AVK_ONLY is only use A2DP.
 **
 ** Returns          0 if successful
 **		     -1 is disconnect failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_disconnect(int type);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_set_visibility
 **
 ** Description     Set the Device Visibility and connectable.
 **
 ** Parameters      discoverable: FALSE if not discoverable, TRUE is discoverable
 **                 connectable:  FALSE if not connectable, TRUE is connectable
 **
 ** Returns         0 if success, -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_set_visibility(BOOLEAN discoverable, BOOLEAN connectable);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_read_remote_device_info
 **
 ** Description      This function is used to read the XML bluetooth remote device file
 **
 ** Parameters
 **
 ** Returns          void
 **
 *******************************************************************************/
extern tAPP_XML_REM_DEVICE *mozart_bluetooth_read_remote_device_info();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_unpair_remote_device
 **
 ** Description      Unpair a remote device, delete device info from bt_devices.xml
 **
 ** Parameters
 **
 ** Returns          0 if success / -1 if error
 **
 *******************************************************************************/
extern int mozart_bluetooth_unpair_remote_device(BD_ADDR *addr);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_set_link_status
 **
 ** Description      Bluetooth set current link status
 **
 ** Parameters       void
 **
 ** Returns          0: success, -1: FALSE
 **
 *******************************************************************************/
extern int mozart_bluetooth_set_link_status(bsa_link_status bt_status);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_get_link_status
 **
 ** Description      Bluetooth get current link status
 **
 ** Parameters       void
 **
 ** Returns          bsa_link_status
 **
 *******************************************************************************/
extern bsa_link_status mozart_bluetooth_get_link_status(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_get_link_down_status
 **
 ** Description      get link down status
 **
 **
 ** Parameter        None
 **
 ** Return Value:    Reason code 19: Mobile terminal to close Bluetooth
 ** 		     Reason code 8: connection timeout
 **		     other Reason code, please see Bluetooth Core spec
 *******************************************************************************/
extern UINT8 mozart_bluetooth_get_link_down_status();

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_get_cod_string
 **
 ** Description     This function is used to get readable string from Class of device
 **
 ** Parameters      class_of_device: The Class of device to decode
 **
 ** Returns         Pointer on string containing device type
 **
 *******************************************************************************/
extern char *mozart_bluetooth_get_cod_string(const DEV_CLASS class_of_device);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_parse_eir_manuf_specific
 **
 ** Description      This function is used to parse EIR manufacturer data
 **
 ** Returns          0 if successful, Otherwise failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_parse_eir_manuf_specific(UINT8 *p_eir, bsa_manu_data *manu_data);



/******************************* Discovery Interface **************************************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_disc_start_regular
 **
 ** Description      Start Device discovery
 **
 ** Returns          0 is ok, other is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_disc_start_regular(int duration);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_disc_stop_regular
 **
 ** Description      Abort Device discovery
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_disc_stop_regular(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_disc_start_services
 **
 ** Description      Start Device services discovery
 **
 ** Parameters      services: Search for specific services supported by the Device
 **		    duration: Multiple of 1.28 seconds
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_disc_start_services(tBSA_SERVICE_MASK services, int duration);

/*******************************************************************************
**
** Function         mozart_bluetooth_disc_stop_services
**
** Description      Abort Device services discovery
**
** Returns          status: 0 if success / -1 otherwise
**
*******************************************************************************/
extern int mozart_bluetooth_disc_stop_services();

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_disc_get_device_count
 **
 ** Description     Get bluetooth search bt devices count
 **
 ** Parameters      void
 **
 ** Returns         > 0 : bt devices number, == 0 : no bt devices,
 **
 *******************************************************************************/
extern unsigned int mozart_bluetooth_disc_get_device_count(void);

/******************************************************************************
 **
 ** Function         mozart_bluetooth_disc_get_device_info
 **
 ** Description      get all discovery bt device info
 **
 ** Parameter        NULL
 **
 ** Returns          tAPP_DISCOVERY_CB addr
 *******************************************************************************/
extern tAPP_DISCOVERY_CB *mozart_bluetooth_disc_get_device_info();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_disc_find_device_name
 **
 ** Description      Find a device in the list of discovered devices
 **
 ** Parameters       index_device
 **
 ** Returns          index_device if successful, -1 is failed.
 **
 *******************************************************************************/
extern int mozart_bluetooth_disc_find_device_name(char *name);



/******************************  HS Servie Interface  *********************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_start_service
 **
 ** Description      Start bluetooth HS service
 **
 ** Parameters	     void
 **
 ** Returns          0 is ok, -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_start_service(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_stop_service
 **
 ** Description      Stop bluetooth HS service
 **
 ** Parameters	     void
 **
 ** Returns          0 is ok, -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_stop_service(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_open
 **
 ** Description      Establishes mono headset connections
 **
 ** Parameter        BD address to connect to. If its NULL, the app will prompt user for device.
 **
 ** Returns          0 if success -1 if failure
 *******************************************************************************/
extern int mozart_bluetooth_hs_open(BSA_OPEN_TYPE choice, unsigned int select_num);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_close
 **
 ** Description      release mono headset connections
 **
 ** Returns          0 if success -1 if failure
 *******************************************************************************/
extern int mozart_bluetooth_hs_close();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_audio_open
 **
 ** Description      Open the SCO connection alone
 ** 		     switch the phone call between the phone to the units
 **
 ** Parameter        None
 **
 ** Returns          0 if success, -1 if failure
 *******************************************************************************/
extern int mozart_bluetooth_hs_audio_open();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_audio_close
 **
 ** Description      Close the SCO connection alone
 ** 		     switch the phone call between the units to the phone
 **
 ** Parameter        None
 **
 ** Returns          0 if success, -1 if failure
 *******************************************************************************/
extern int mozart_bluetooth_hs_audio_close();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_get_call_state
 **
 ** Description      Access to bluetooth calls state
 **
 ** Parameters	     void
 **
 ** Returns          see BSA_CALL_STATE
 **
 *******************************************************************************/
extern BSA_CALL_STATE mozart_bluetooth_hs_get_call_state(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_answer_call
 **
 ** Description      Bluetooth answer the call
 **
 ** Parameters	     void
 **
 ** Returns          0 if OK, -1 otherwise.
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_answer_call(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_hangup_call
 **
 ** Description      Bluetooth hang up the call
 **
 ** Parameters	     void
 **
 ** Returns          0 if OK, -1 otherwise.
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_hangup_call(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_hold_call
 **
 ** Description      Hold active call
 **
 ** Parameters       void
 **
 ** Returns          0 if successful execution, error code else
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_hold_call(tBSA_BTHF_CHLD_TYPE_T type);

/*******************************************************************************
 **
 ** Function         mozart_blutooth_hs_set_volume
 **
 ** Description      Send volume AT Command
 **
 ** Parameters       type: speaker or microphone, volume: speaker or microphone volume value
 **
 ** Returns          0 if successful execution, error code else
 **
 *******************************************************************************/
extern int mozart_blutooth_hs_set_volume(tBSA_BTHF_VOLUME_TYPE_T type, int volume);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hs_send_battery_level
 **
 ** Description      Send bt battery_value to phone device
 **
 ** Parameters       unsigned int battery_value: 0~9
 **		     0 is the minimum value, 9 is the maximum value
 **
 ** Returns          0 if successful execution, error code else
 **
 *******************************************************************************/
extern int mozart_bluetooth_hs_send_battery_level(unsigned int battery_value);

/* HS callback events */
#define BTA_HS_ENABLE_EVT           0  /* HS enabled */
#define BTA_HS_DISABLE_EVT          1  /* HS Disabled */
#define BTA_HS_REGISTER_EVT         2  /* HS Registered */
#define BTA_HS_DEREGISTER_EVT       3  /* HS Registered */
#define BTA_HS_OPEN_EVT             4 /* HS connection open or connection attempt failed  */
#define BTA_HS_CLOSE_EVT            5 /* HS connection closed */
#define BTA_HS_CONN_EVT             6 /* HS Service Level Connection is UP */
#define BTA_HS_CONN_LOSS_EVT        7 /* Link loss of connection to audio gateway happened */
#define BTA_HS_AUDIO_OPEN_REQ_EVT   8 /* Audio open request*/
#define BTA_HS_AUDIO_OPEN_EVT       9 /* Audio connection open */
#define BTA_HS_AUDIO_CLOSE_EVT      10/* Audio connection closed */
#define BTA_HS_CIND_EVT             11/* Indicator string from AG */
#define BTA_HS_CIEV_EVT             12/* Indicator status from AG */
#define BTA_HS_RING_EVT             13/* RING alert from AG */
#define BTA_HS_CLIP_EVT             14/* Calling subscriber information from AG */
#define BTA_HS_BSIR_EVT             15/* In band ring tone setting */
#define BTA_HS_BVRA_EVT             16/* Voice recognition activation/deactivation */
#define BTA_HS_CCWA_EVT             17/* Call waiting notification */
#define BTA_HS_CHLD_EVT             18/* Call hold and multi party service in AG */
#define BTA_HS_VGM_EVT              19/* MIC volume setting */
#define BTA_HS_VGS_EVT              20/* Speaker volume setting */
#define BTA_HS_BINP_EVT             21/* Input data response from AG */
#define BTA_HS_BTRH_EVT             22/* CCAP incoming call hold */
#define BTA_HS_CNUM_EVT             23/* Subscriber number */
#define BTA_HS_COPS_EVT		        24/* Operator selection info from AG */
#define BTA_HS_CMEE_EVT		        25/* Enhanced error result from AG */
#define BTA_HS_CLCC_EVT             26/* Current active call list info */
#define BTA_HS_UNAT_EVT             27/* AT command response fro AG which is not specified in HFP or HSP */
#define BTA_HS_OK_EVT               28 /* OK response */
#define BTA_HS_ERROR_EVT            29 /* ERROR response */
#define BTA_HS_BCS_EVT              30 /* Codec selection from AG */

/* BSA HS callback events */
typedef enum
{
	BSA_HS_OPEN_EVT,                    /* HS connection open or connection attempt failed  */
	BSA_HS_CLOSE_EVT,                   /* HS connection closed */
	BSA_HS_CONN_EVT,                    /* HS Service Level Connection is UP */
	BSA_HS_CONN_LOSS_EVT,               /* Link loss of connection to audio gateway happened */
	BSA_HS_AUDIO_OPEN_REQ_EVT,          /* Audio open request*/
	BSA_HS_AUDIO_OPEN_EVT,              /* Audio connection open */
	BSA_HS_AUDIO_CLOSE_EVT,             /* Audio connection closed */

	BSA_HS_CIND_EVT  = BTA_HS_CIND_EVT, /* Indicator string from AG */
	BSA_HS_CIEV_EVT  = BTA_HS_CIEV_EVT, /* Indicator status from AG */
	BSA_HS_RING_EVT  = BTA_HS_RING_EVT, /* RING alert from AG */
	BSA_HS_CLIP_EVT  = BTA_HS_CLIP_EVT, /* Calling subscriber information from AG */
	BSA_HS_BSIR_EVT  = BTA_HS_BSIR_EVT, /* In band ring tone setting */
	BSA_HS_BVRA_EVT  = BTA_HS_BVRA_EVT, /* Voice recognition activation/deactivation */
	BSA_HS_CCWA_EVT  = BTA_HS_CCWA_EVT, /* Call waiting notification */
	BSA_HS_CHLD_EVT  = BTA_HS_CHLD_EVT, /* Call hold and multi party service in AG */
	BSA_HS_VGM_EVT   = BTA_HS_VGM_EVT,  /* MIC volume setting */
	BSA_HS_VGS_EVT   = BTA_HS_VGS_EVT,  /* Speaker volume setting */
	BSA_HS_BINP_EVT  = BTA_HS_BINP_EVT, /* Input data response from AG */
	BSA_HS_BTRH_EVT  = BTA_HS_BTRH_EVT, /* CCAP incoming call hold */
	BSA_HS_CNUM_EVT  = BTA_HS_CNUM_EVT, /* Subscriber number */
	BSA_HS_COPS_EVT  = BTA_HS_COPS_EVT, /* Operator selection info from AG */
	BSA_HS_CMEE_EVT  = BTA_HS_CMEE_EVT, /* Enhanced error result from AG */
	BSA_HS_CLCC_EVT  = BTA_HS_CLCC_EVT, /* Current active call list info */
	BSA_HS_UNAT_EVT  = BTA_HS_UNAT_EVT, /* AT command response fro AG which is not specified in HFP or HSP */
	BSA_HS_OK_EVT    = BTA_HS_OK_EVT,   /* OK response */
	BSA_HS_ERROR_EVT = BTA_HS_ERROR_EVT,/* ERROR response */
	BSA_HS_BCS_EVT   = BTA_HS_BCS_EVT,  /* Codec selection from AG */
} tBSA_HS_EVT;

typedef struct {
	UINT32 rate;
	UINT32 channel;
	UINT32 bits;
} hs_sample_info;

typedef struct {
	hs_sample_info hope;
	hs_sample_info actual;
	UINT32 volume;
} hs_sample_param;

typedef struct {
	char	*ibuf;
	char	*obuf;
	UINT32	ilen;
	UINT32	olen;
} hs_stream_in_info;

typedef struct {
	char	*ref_buf;
	char	*record_buf;
	char	*out_buf;
	UINT32	ref_len;
	UINT32	record_len;
	UINT32	out_len;
} hs_stream_out_info;

typedef struct {
	hs_stream_in_info  in;
	hs_stream_out_info out;
	hs_sample_param	ref;
	hs_sample_param	record;
	hs_sample_param	aec;
	hs_sample_param	hs;
	hs_sample_param	dsp;
	UINT32 sample_time;	/* ms */
} hs_stream_msg;

typedef struct {
	int  dsp_r_fd;
	int  dsp_w_fd;
	int  bt_r_fd;
	int  bt_w_fd;
	int  record_r_fd;

	int  write_bt_call;
	int  read_bt_call;
	int  write_codec_call;
	int  read_dmic_call;
	hs_stream_msg hs_msg;
} device_prope;

typedef int (*hs_evt_cback_t)(tBSA_HS_EVT event, void *p_data);
typedef int (*hs_data_cback_t)(hs_stream_msg *hs_msg);
typedef struct {
	hs_evt_cback_t  mozart_hs_evt_cback;
	hs_data_cback_t mozart_hs_data_in_cback;
	hs_data_cback_t mozart_hs_data_out_cback;
} hs_cback_init_data;

extern void mozart_bluetooth_hs_init_callback(hs_cback_init_data *cback_data);

/******************************** AVK Servie Interface ***********************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_start_service
 **
 ** Description      Start bluetooth A2DP service
 **
 ** Parameters	     void
 **
 ** Returns          0 if OK, -1 otherwise.
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_start_service(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_stop_service
 **
 ** Description      Stop bluetooth A2DP service
 **
 ** Parameters	     void
 **
 ** Returns          0 if OK, -1 otherwise.
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_stop_service(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_start_play
 **
 ** Description      Bluetooth start and resume play
 **
 ** Parameters	     void
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_start_play(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_stop_play
 **
 ** Description      Bluetooth stop the current play
 **
 ** Parameters	     void
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_stop_play(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_play_pause
 **
 ** Description      Bluetooth execute play and pause command switch Automatically
 **
 ** Parameters	     void
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_play_pause(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_play
 **
 ** Description      Bluetooth execute play command
 **
 ** Parameters	     void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_play(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_pause
 **
 ** Description      Bluetooth execute pause command
 **
 ** Parameters	     void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_pause(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_next_music
 **
 ** Description      Bluetooth play the next music
 **
 ** Parameters	     void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_next_music(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_prev_music
 **
 ** Description      Bluetooth play the previous music
 **
 ** Parameters	     void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_prev_music(void);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_avk_get_play_state
 **
 ** Description     get avk plat status
 **
 ** Parameters      void
 **
 ** Returns          TRUE: play   FALSE: stop
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_get_play_state(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_set_volume
 **
 ** Description      This function sends absolute volume
 **
 ** Paraneters       volume: 0~0x7F
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_set_volume(UINT8 volume);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_app_avk_open
 **
 ** Description      Example of function to open AVK connection
 **
 ** Parameters       choice: default BSA_APP_RECONNECT,
 **		     select_num: 0 is the last connected Device
 ** Returns          0 is success, other is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_open(BSA_OPEN_TYPE choice, unsigned int select_num);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_close
 **
 ** Description      Function to close AVK connection
 **
 ** Returns          0 is success, otherwise is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_close();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_init_callback
 **
 ** Description
 **
 ** Parameters       avk_cback_init_data
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_init_callback(avk_cback_init_data *cback_data);


/********************************** OPP Interface ****************************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ops_start
 **
 ** Description      start OPP Server application
 **
 ** Parameters       void
 **
 ** Returns          BSA_SUCCESS success, error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_ops_start(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ops_stop
 **
 ** Description      stop OPP Server application
 **
 ** Parameters       void
 **
 ** Returns          BSA_SUCCESS success, error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_ops_stop(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ops_auto_accept
 **
 ** Description      set ops auto accept object
 **
 ** Parameters       struct ops_accept_param
 **
 ** Returns          BSA_SUCCESS success, error code for failure
 **
 *******************************************************************************/
tBSA_STATUS mozart_bluetooth_ops_auto_accept(ops_accept_param *ops_param);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ops_get_object_name
 **
 ** Description      get file name when send request
 **
 ** Parameters       NULL
 **
 ** Returns          object file name
 **
 *******************************************************************************/
extern char *mozart_bluetooth_ops_get_object_name();

/*******************************************************************************
 **
 ** function         mozart_bluetooth_opc_start
 **
 ** description      start opp client application
 **
 ** parameters       void
 **
 ** returns          bsa_success success, error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_opc_start(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_opc_stop
 **
 ** Description      stop OPP Client application
 **
 ** Parameters       void
 **
 ** Returns          BSA_SUCCESS success, error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_opc_stop(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_opc_set_key
 **
 ** Description      Select key to set opc Function
 **
 ** Parameters       APP_OPC_KEY_XXX
 **
 ** Returns          BSA_SUCCESS success, error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_opc_set_key(int choice);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_opc_push_file
 **
 ** Description      Example of function to push a file to current peer device
 **
 ** Parameters       struct opc_push_param
 **
 ** Returns          BSA_SUCCESS success error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_opc_push_file(opc_push_param *opc_param);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_opc_disconnect
 **
 ** Description      Example of function to disconnect current device
 **
 ** Parameters       void
 **
 ** Returns          BSA_SUCCESS success error code for failure
 **
 *******************************************************************************/
extern tBSA_STATUS mozart_bluetooth_opc_disconnect(void);

/********************************* BLE Interface **************************************/

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_start
 **
 ** Description     start BSA BLE Function
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_start();

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_close
 **
 ** Description     Close BSA BLE Function
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_close();

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_set_visibility
 **
 ** Description     Set the Device BLE Visibility parameters
 **
 ** Parameters      discoverable: FALSE if not discoverable
 **                 connectable: FALSE if not connectable
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_set_visibility(BOOLEAN discoverable, BOOLEAN connectable);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ble_start_regular
 **
 ** Description      Start BLE Device discovery
 **
 ** Returns          0 if success, -1 if failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_start_regular(int duration);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_configure_advertisement_data
 **
 ** Description     start BLE advertising
 **
 ** Parameters      struct tBSA_DM_BLE_ADV_CONFIG
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_configure_adv_data(tBSA_DM_BLE_ADV_CONFIG *adv_data);

/*******************************************************************************
 **
 ** Function       mozart_bluetooth_ble_set_adv_param
 **
 ** Description     Configure the BLE Prefer Advertisement parameters
 **
 ** Parameters      pointer to tBSA_DM_BLE_ADV_PARAM
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_set_adv_param(tBSA_DM_BLE_ADV_PARAM *p_req);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_set_conn_param
 **
 ** Description     Configure the BLE Prefer Conn parameters
 **
 ** Parameters      pointer to tBSA_DM_BLE_CONN_PARAM
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_set_conn_param(tBSA_DM_BLE_CONN_PARAM *p_req);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_get_cb
 **
 ** Description     get global struct app_ble_cb
 **
 *******************************************************************************/
extern tAPP_BLE_CB *mozart_bluetooth_ble_get_cb();

/*
 * BLE Server functions
 */
/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_server_register
 **
 ** Description     Register server app
 **
 ** Parameters      uuid
 **
 ** Returns         status: server_num if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_register(UINT16 uuid);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_server_deregister
 **
 ** Description     Deregister server app
 **
 ** Parameters      server_num: server number
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_deregister(int server_num);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_server_create_service
 **
 ** Description     create GATT service
 **
 ** Parameters      struct ble_service_info
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_create_service(ble_create_service_data *ble_create_service_data);

/*******************************************************************************
 **
 ** Function       mozart_bluetooth_ble_server_start_service
 **
 ** Description     Start Service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_start_service(ble_start_service_data *ble_start_service_data);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_add_character
 **
 ** Description     Add character to service
 **
 ** Parameters      struct bsa_ble_add_character
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_add_character(ble_char_data *ble_char_data);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_ble_server_display
 **
 ** Description      display BLE server
 **
 ** Parameters
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_ble_server_display();

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_server_send_indication
 **
 ** Description     Send indication to client
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_server_send_indication(ble_server_indication *ble_indication);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_server_set_char_value
 **
 ** Description      Set BLE character value
 **
 ** Parameter        server_num: service num. character_num: character num. value: character value.
 **
 ** Returns          0 if service enabled, -1 if service not enabled
 *******************************************************************************/
extern int mozart_bluetooth_server_set_char_value(int server_num, int character_num, UINT8 *value, int vaule_num);

/* ---------------- ble client ------------------------------- */

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_register
 **
 ** Description     This is the ble client register command
 **
 ** Parameters      UINT16 uuid
 **
 ** Returns         status: client_num if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_register(UINT16 uuid);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_deregister
 **
 ** Description     This is the ble deregister app
 **
 ** Parameters      int client_num
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_deregister(int client_num);


/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_connect_server
 **
 ** Description     This is the ble open connection to ble server
 **
 ** Parameters      struct ble_client_connect_data
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_connect_server(ble_client_connect_data *cl_connect_data);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_disconnect_server
 **
 ** Description     This is the ble close connection
 **
 ** Parameters      client_num
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_disconnect_server(int client_num);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_read
 **
 ** Description     This is the read function to read a characteristec or characteristic descriptor from BLE server
 **
 ** Parameters      struct ble_client_read_data
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_read(ble_client_read_data *client_data);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_client_write
 **
 ** Description     This is the write function to write a characteristic or characteristic discriptor to BLE server.
 **
 ** Parameters      ble_client_write_data
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_write(ble_client_write_data *cl_write_data);

/*******************************************************************************
 **
 ** Function        app_ble_client_register_notification
 **
 ** Description     This is the register function to receive a notification
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_ble_client_register_notification(BLE_CL_NOTIFREG *cl_notireg);

/*******************************************************************************
 **
 ** Function        mozart_bluetooth_ble_callback
 **
 ** Description     This is the ble callback function when ble init
 **
 ** Parameters      None
 **
 *******************************************************************************/
extern void mozart_bluetooth_ble_callback(bt_ble_callback *bt_ble);

/********************************** BLE HID Interface *******************************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hh_start
 **
 ** Description      Start HID Host
 **
 ** Parameters	     void
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_hh_start(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hh_stop
 **
 ** Description      Stop HID Host
 **
 ** Parameters       void
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_hh_stop(void);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_hh_connect
 **
 ** Description      Example of function to connect to HID device
 **
 ** Parameters	     ble_hh_connect_data
 **
 ** Returns          0 is successed , other is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_hh_connect(ble_hh_connect_data *connect_data);

extern int mozart_bluetooth_hh_callback(bt_hh_func_t hh_cback);


/********************************** A2DP Synk Interface ****************************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_open_dsp
 **
 ** Description      Function to open AVK dsp
 **
 ** Parameters       msec: Fade in time(msec), 0 is not use this function
 **
 ** Returns          0 is success, otherwise is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_open_dsp(unsigned int msec);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_close_dsp
 **
 ** Description      Function to close AVK dsp
 **
 ** Parameters       msec: Fade out time(msec), 0 is not use this function
 **
 ** Returns          0 is success, otherwise is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_close_dsp(unsigned int msec);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_send_get_element_att_cmd
 **
 ** Description      send BSA_AVK_RC_VD_GET_ELEMENT_ATTR cmd
 **
 ** Returns          0 if success, other is failed
 **
 *******************************************************************************/
extern int mozart_bluetooth_avk_send_get_element_att_cmd();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_get_element_att
 **
 ** Description      get current set values on the target
 ** 		     for the provided player application setting attributes list
 **
 ** Returns          struct tBSA_AVK_GET_ELEMENT_ATTR_MSG
 **
 *******************************************************************************/
extern tBSA_AVK_GET_ELEMENT_ATTR_MSG *mozart_bluetooth_avk_get_element_att();

/*******************************************************************************
 **
 ** Function	     mozart_bluetooth_avk_close_arvcp_function
 **
 ** Description      This function close avrcp Features
 **
 ** Returns          None
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_close_arvcp_function();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_send_get_play_status_command
 **
 ** Description      send get_play_status cmd
 **
 ** Returns          void
 **
 *******************************************************************************/
extern void mozart_bluetooth_avk_send_get_play_status_command();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_avk_get_play_status
 **
 ** Description      get current play status
 **
 ** Returns	     tBSA_AVK_GET_PLAY_STATUS_MSG
 **
 *******************************************************************************/
extern tBSA_AVK_GET_PLAY_STATUS_MSG *mozart_bluetooth_avk_get_play_status();

/********************************** A2DP Source Interface ****************************************/

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_start_service
 **
 ** Description      Start bluetooth A2DP Source service
 **
 ** Parameters	     char *playlist_patch
 **
 ** Returns          0 if OK, -1 otherwise.
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_start_service(char *playlist_patch);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_display_connections
 **
 ** Description      This function displays the connections
 **
 ** Returns          status
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_display_connections();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_open
 **
 ** Description      Function to open AV connection
 **
 ** Parameters       struct av_open_param
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_open(av_open_param *av_param);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_open_by_addr
 **
 ** Description      Function to open AV connection
 **
 ** Parameters       BD_ADDR of the deivce to connect
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_open_by_addr(BD_ADDR *addr);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_close
 **
 ** Description      Function to close AV connection
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_close();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_play_playlist
 **
 ** Description      Example of start to play a play list
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_play_playlist();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_stop_play
 **
 ** Description      Stop playing completely
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_stop_play();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_pause_play
 **
 ** Description      Pause playing
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_pause_play();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_resume_play
 **
 ** Description      Resume playing
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_resume_play();

/*******************************************************************************
 **
 ** Function         app_av_rc_send_absolute_volume_vd_command
 **
 ** Description      Example of Send a Vendor Specific command
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_rc_send_abs_vol_command(int volume);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_rc_send_commond
 **
 ** Description      Example of Send a RemoteControl command
 **
 ** Parameters	     BSA_AV_RC_XX
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_rc_send_commond(int command);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_start_stream
 **
 ** Description      Start playing current stream
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_start_stream(tBSA_AV_MEDIA_FEED_CFG_PCM *pcm_config);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_stop_stream
 **
 ** Description      Stop playing current stream
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_stop_stream();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_send_stream
 **
 ** Description      Send pcm data to the AV channel
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_send_stream(void *audio_buf, int length);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_get_play_state
 **
 ** Description      get av current play state
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_get_play_state();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_set_busy_level
 **
 ** Description      Change busy level
 **
 ** Returns          0 if successful, error code otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_av_set_busy_level(UINT8 level);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_av_get_cur_connected_device
 **
 ** Description      get av current connected devices info
 **
 *******************************************************************************/
extern tAPP_XML_REM_DEVICE *mozart_bluetooth_av_get_cur_connected_device();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_start
 **
 ** Description      Start DG application
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_start();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_stop
 **
 ** Description      Stop DG application
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_stop();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_open_connection
 **
 ** Description      Example of function to open a DG connection for multiple
 **                  or single connections.
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_open_connection(dg_open_paramters *dg_parm);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_close_connection
 **
 ** Description      Example of function to close a DG connection
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_close_connection();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_start_server
 **
 ** Description      Example of function to start an SPP server
 **
 ** Returns          status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_start_server(dg_start_service_paramters *dg_service);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_stop_server
 **
 ** Description      Example of function to stop an SPP server
 **
 ** Returns          status
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_stop_server();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_send_file
 **
 ** Description      Example of function to send a file
 **
 ** Returns          status: -1 in case of error, 0 if successful
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_send_file(char *file_name);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_dg_send
 **
 ** Description      Example of function to send data
 **
 ** Parameters	     connection: default 0
 **
 ** Returns          status: -1 in case of error, 0 if successful
 **
 *******************************************************************************/
extern int mozart_bluetooth_dg_send(int connection, char *data, int data_len);

extern int mozart_bluetooth_dg_callback(bt_dg_func_t cback);

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_pbc_start
 **
 ** Description      Example of function to enable PBC functionality
 **
 ** Parameters	     void
 **
 ** Returns          BSA_SUCCESS is ok, Otherwise is failed
 **
 *******************************************************************************/
tBSA_STATUS mozart_bluetooth_pbc_start();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_pbc_stop
 **
 ** Description      Example of function to stop PBC functionality
 **
 ** Parameters
 **
 ** Returns          BSA_SUCCESS is ok, Otherwise is failed
 **
 *******************************************************************************/
tBSA_STATUS mozart_bluetooth_pbc_stop();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_pbc_open_connection
 **
 ** Description      Example of function to open up a connection to peer
 **
 ** Parameters
 **
 ** Returns          BSA_SUCCESS is ok, Otherwise is failed
 **
 *******************************************************************************/
int mozart_bluetooth_pbc_open_connection();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_pbc_close_connection
 **
 ** Description      Example of function to disconnect current connection
 **
 ** Parameters
 **
 ** Returns          BSA_SUCCESS is ok, Otherwise is failed
 **
 *******************************************************************************/
tBSA_STATUS mozart_bluetooth_pbc_close_connection();

/*******************************************************************************
 **
 ** Function         mozart_bluetooth_pbc_get_phonebook
 **
 ** Description      Example of function to perform a get phonebook operation
 **
 ** Parameters
 **
 ** Returns          BSA_SUCCESS is ok, Otherwise is failed
 **
 *******************************************************************************/
int mozart_bluetooth_pbc_get_phonebook(char *file_name);

#endif
