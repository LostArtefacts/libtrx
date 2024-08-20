#include "virtual_file.h"

#include "memory.h"

#include <assert.h>
#include <string.h>

VFILE *VFile_CreateFromBuffer(const char *data, size_t size)
{
    VFILE *const file = Memory_Alloc(sizeof(VFILE));
    file->content = Memory_Dup(data, size);
    file->size = size;
    file->cur_ptr = file->content;
    return file;
}

void VFile_Close(VFILE *file)
{
    Memory_FreePointer(&file->content);
    Memory_FreePointer(&file);
}

size_t VFile_GetPos(const VFILE *file)
{
    return file->cur_ptr - file->content;
}

void VFile_Skip(VFILE *file, int32_t offset)
{
    const size_t cur_pos = VFile_GetPos(file);
    assert(cur_pos + offset <= file->size);
    file->cur_ptr += offset;
}

void VFile_Read(VFILE *file, void *target, size_t size)
{
    const size_t cur_pos = VFile_GetPos(file);
    assert(cur_pos + size <= file->size);
    memcpy(target, file->cur_ptr, size);
    file->cur_ptr += size;
}

int8_t VFile_ReadS8(VFILE *file)
{
    int8_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}

int16_t VFile_ReadS16(VFILE *file)
{
    int16_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}

int32_t VFile_ReadS32(VFILE *file)
{
    int32_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}

uint8_t VFile_ReadU8(VFILE *file)
{
    uint8_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}

uint16_t VFile_ReadU16(VFILE *file)
{
    uint16_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}

uint32_t VFile_ReadU32(VFILE *file)
{
    uint32_t result;
    VFile_Read(file, &result, sizeof(result));
    return result;
}
