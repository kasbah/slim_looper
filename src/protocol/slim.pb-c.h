/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_slim_2eproto__INCLUDED
#define PROTOBUF_C_slim_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _Command Command;
typedef struct _Setting Setting;
typedef struct _LooperMessage LooperMessage;


/* --- enums --- */

typedef enum _LooperMessage__Type {
  LOOPER_MESSAGE__TYPE__COMMAND = 0,
  LOOPER_MESSAGE__TYPE__SETTING = 1
} LooperMessage__Type;
typedef enum _LooperCommand {
  LOOPER_COMMAND__PLAY_OR_PAUSE = 0,
  LOOPER_COMMAND__RECORD = 1,
  LOOPER_COMMAND__OVERDUB = 2,
  LOOPER_COMMAND__INSERT = 3,
  LOOPER_COMMAND__REPLACE = 4
} LooperCommand;

/* --- messages --- */

struct  _Command
{
  ProtobufCMessage base;
  LooperCommand command;
};
#define COMMAND__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&command__descriptor) \
    , 0 }


struct  _Setting
{
  ProtobufCMessage base;
  protobuf_c_boolean has_dry;
  float dry;
};
#define SETTING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&setting__descriptor) \
    , 0,0 }


struct  _LooperMessage
{
  ProtobufCMessage base;
  LooperMessage__Type type;
  Command *command;
  Setting *setting;
};
#define LOOPER_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&looper_message__descriptor) \
    , 0, NULL, NULL }


/* Command methods */
void   command__init
                     (Command         *message);
size_t command__get_packed_size
                     (const Command   *message);
size_t command__pack
                     (const Command   *message,
                      uint8_t             *out);
size_t command__pack_to_buffer
                     (const Command   *message,
                      ProtobufCBuffer     *buffer);
Command *
       command__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   command__free_unpacked
                     (Command *message,
                      ProtobufCAllocator *allocator);
/* Setting methods */
void   setting__init
                     (Setting         *message);
size_t setting__get_packed_size
                     (const Setting   *message);
size_t setting__pack
                     (const Setting   *message,
                      uint8_t             *out);
size_t setting__pack_to_buffer
                     (const Setting   *message,
                      ProtobufCBuffer     *buffer);
Setting *
       setting__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   setting__free_unpacked
                     (Setting *message,
                      ProtobufCAllocator *allocator);
/* LooperMessage methods */
void   looper_message__init
                     (LooperMessage         *message);
size_t looper_message__get_packed_size
                     (const LooperMessage   *message);
size_t looper_message__pack
                     (const LooperMessage   *message,
                      uint8_t             *out);
size_t looper_message__pack_to_buffer
                     (const LooperMessage   *message,
                      ProtobufCBuffer     *buffer);
LooperMessage *
       looper_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   looper_message__free_unpacked
                     (LooperMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Command_Closure)
                 (const Command *message,
                  void *closure_data);
typedef void (*Setting_Closure)
                 (const Setting *message,
                  void *closure_data);
typedef void (*LooperMessage_Closure)
                 (const LooperMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    looper_command__descriptor;
extern const ProtobufCMessageDescriptor command__descriptor;
extern const ProtobufCMessageDescriptor setting__descriptor;
extern const ProtobufCMessageDescriptor looper_message__descriptor;
extern const ProtobufCEnumDescriptor    looper_message__type__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_slim_2eproto__INCLUDED */
