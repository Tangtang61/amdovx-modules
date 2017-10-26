#ifndef INFCOM_H
#define INFCOM_H

// Configure Protocol:
//    client: (connect)
//  * server: InfComCommand:INFCOM_CMD_SEND_MODE
//    client: InfComCommand:INFCOM_CMD_SEND_MODE with data={INFCOM_MODE_CONFIGURE}
//  * server: InfComCommand:INFCOM_CMD_CONFIG_INFO with data={numModels,maxGPUs}
//    client: InfComCommand:INFCOM_CMD_CONFIG_INFO with data={numModels,maxGPUs}
//  * server: InfComCommand:INFCOM_CMD_MODEL_INFO with data={iw,ih,ic,ow,oh,oc} message=modelName
//    client: InfComCommand:INFCOM_CMD_MODEL_INFO with data={iw,ih,ic,ow,oh,oc} message=modelName
//  * server: (repeat INFCOM_CMD_MODEL_INFO for each of numModels)
//  * client: InfComCommand:INFCOM_CMD_DONE
//  * server: InfComCommand:INFCOM_CMD_DONE
//    client: (disconnect)

// Compiler Protocol:
//    client: (connect)
//  * server: InfComCommand:INFCOM_CMD_SEND_MODE
//    client: InfComCommand:INFCOM_CMD_SEND_MODE with data={INFCOM_MODE_COMPILER,iw,ih,ic,mode} [message=options]
//  * server: InfComCommand:INFCOM_CMD_SEND_MODELFILE1
//    client: InfComCommand:INFCOM_CMD_SEND_MODELFILE1 with data[0]=size-in-bytes message=fileName
//    client: <byte-stream-of-prototxt> <eof-marker:32-bit>
//  * server: InfComCommand:INFCOM_CMD_SEND_MODELFILE2
//    client: InfComCommand:INFCOM_CMD_SEND_MODELFILE2 with data[0]=size-in-bytes message=fileName
//    client: <byte-stream-of-caffemodel> <eof-marker:32-bit>
//  * server: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//    client: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//  * server: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//    client: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//  * server: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//    client: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={0,progress,0,0,0} message=log
//  * server: (same type of messages with status updates: in case of error: data[0] will contain -ve eror code)
//    client: (reply the same message)
//  * server: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={1,100,ow,oh,oc} message=modelName
//    client: InfComCommand:INFCOM_CMD_COMPILER_STATUS with data={1,100,ow,oh,oc} message=modelName
//  * server: InfComCommand:INFCOM_CMD_DONE
//    client: InfComCommand:INFCOM_CMD_DONE
//    client: (disconnect)

// Inference Run Protocol:
//    client: (connect)
//  * server: InfComCommand:INFCOM_MODE_INFERENCE
//    client: InfComCommand:INFCOM_CMD_SEND_MODE with data={INFCOM_MODE_INFERENCE,GPUs,iw,ih,ic,ow,oh,oc} message="modelName [options]"
//  * server: InfComCommand:INFCOM_CMD_SEND_IMAGES with data={maxCount}
//    client: InfComCommand:INFCOM_CMD_SEND_IMAGES with data={count} -- count:0..maxCount
//    client: for each image: { <tag:32-bit> <size:32-bit> <byte-stream> <eof-marker:32-bit> }
//  * server: InfComCommand:INFCOM_CMD_INFERENCE_RESULT data={imageCount,0,<tag1>,<label1>,<tag2>,<label2>,...} upto 14 tags
//    client: InfComCommand:INFCOM_CMD_INFERENCE_RESULT data={imageCount,0,<tag1>,<label1>,<tag2>,<label2>,...} upto 14 tags
//  * server: (repeat of INFCOM_CMD_SEND_IMAGES and INFCOM_CMD_INFERENCE_RESULT messages)
//    client: (repeat of INFCOM_CMD_SEND_IMAGES and INFCOM_CMD_INFERENCE_RESULT messages)
//  * server: InfComCommand:INFCOM_CMD_DONE
//    client: InfComCommand:INFCOM_CMD_DONE
//    client: (disconnect)

// InfComCommand.magic
#define INFCOM_MAGIC                           0x02388e50

// InfComCommand.command
#define INFCOM_CMD_DONE                        0
#define INFCOM_CMD_SEND_MODE                   1
#define INFCOM_CMD_CONFIG_INFO                 101
#define INFCOM_CMD_MODEL_INFO                  102
#define INFCOM_CMD_SEND_MODELFILE1             201
#define INFCOM_CMD_SEND_MODELFILE2             202
#define INFCOM_CMD_COMPILER_STATUS             203
#define INFCOM_CMD_INFERENCE_INITIALIZATION    301
#define INFCOM_CMD_SEND_IMAGES                 302
#define INFCOM_CMD_INFERENCE_RESULT            303

// InfComCommand.data[0] for INFCOM_CMD_SEND_MODE
#define INFCOM_MODE_CONFIGURE                  1
#define INFCOM_MODE_COMPILER                   2
#define INFCOM_MODE_INFERENCE                  3

// EOF marker
#define INFCOM_EOF_MARKER                      0x12344321

// Max images per packet
#define INFCOM_MAX_IMAGES_PER_PACKET           6

// Max packet size
#define INFCOM_MAX_PACKET_SIZE                 8192

// InfComCommand for message exchange:
//    all unspecified values must be ZEROs
typedef struct {
    int magic;
    int command;
    int data[14];
    char message[64];
} InfComCommand;

#endif
