#ifndef _SOUNDX_H
#define _SOUNDX_H

enum SoundEx {};

#ifdef __cplusplus
extern "C" {
#endif

  enum SoundEx String2SoundEx(const char *name);
  char *SoundEx2String(enum SoundEx sx);

#ifdef __cplusplus
};
#endif

#endif
