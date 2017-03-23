#ifndef HciUTH
#define HciUTH

#include "hci_micarray.h"

#ifdef __cplusplus
extern "C" {
#endif

HCI_ERR_CODE GetLogLevel(MicArraySession session, int *pLevel);
HCI_ERR_CODE GetMicVolume(MicArraySession session, int *pVolume);
HCI_ERR_CODE GetRefVolume(MicArraySession session, int *pVolume);
HCI_ERR_CODE GetSpeakerVolume(MicArraySession session, int *pVolume);
HCI_ERR_CODE GetWakeCheckMode(MicArraySession session, int *pMode);
HCI_ERR_CODE GetAECStatus(MicArraySession session, int *pStatus);
HCI_ERR_CODE GetAGCStatus(MicArraySession session, int *pStatus);
HCI_ERR_CODE GetDOAStatus(MicArraySession session, int *pStatus);
HCI_ERR_CODE GetVQEDirection(MicArraySession session, int *pDirection);

HCI_ERR_CODE SetLogLevel(MicArraySession session, int level);
HCI_ERR_CODE SetMicVolume(MicArraySession session, int volume);
HCI_ERR_CODE SetRefVolume(MicArraySession session, int volume);
HCI_ERR_CODE SetSpeakerVolume(MicArraySession session, int volume);
HCI_ERR_CODE SetWakeCheckMode(MicArraySession session, int mode);
HCI_ERR_CODE SetAECStatus(MicArraySession session, int status);
HCI_ERR_CODE SetAGCStatus(MicArraySession session, int status);
HCI_ERR_CODE SetDOAStatus(MicArraySession session, int status);
HCI_ERR_CODE SetVQEDirection(MicArraySession session, int direction);

#ifdef __cplusplus
}
#endif

#endif /* HciUTH */
