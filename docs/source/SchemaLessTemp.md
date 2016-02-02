Schema-less in-place serialization

Almost all serialization formats out there have some pretty big drawbacks:

1. They need to be parsed to be accessed, which takes time.

2. They need to be unpacked into a secondary object representation, which causes:

    1. Allocation/deallocation pressure on the memory management system.

    2. Uses more memory and is less cache friendly than the serialized buffer.

These problems have been solved by [FlatBuffers](https://google.github.io/flatbuffers/), which allows in-place use of serialized data.

For maximum performance and type safety, FlatBuffers is designed entirely around schemas (strongly typed data definition files) however. While it has reflection functionality, it really isn't suitable for use in a schema-less (self-describing) context.

It seems that, against their best interests, large volumes of the world's data are still being stored in schema-less formats, with no sign of improvement.

Can we achieve the no-parsing, no-allocation benefits of FlatBuffers in a schema-less context? What would such an encoding look like?

# Schema-less direct field access.

The distinguishing feature of FlatBuffers is its O(1) field access, which no schema-less formats support (e.g. JSON needs linear parsing up to the desired element to read it).

Clearly this would need to take the form of an array of fields, each element storing a type, a value, and in the case of maps/dictionaries/objects, a field name.

But how many bits wide? The answer is: a variable number of bits! Basically, a "type" is a uint8_t:

* Lower 2 bits: A value that indicates the bit-width of the type that follows (8/16/32/64).

* Middle 4 bits: The base type.

* Top 2 bits: Element type, used for typed vectors.

A base type is then one of:

* A signed/unsigned int stored inline (bit width ignored, determined by parent vector).

* An offset to a signed/unsigned int (of given bit-width).

* A float stored inline (bit width ignored, determined by parent vector). We support not just 32/64 bits floats, but 16 and.. 8! (mostly for completeness).

* An offset to a float (of given bit-width).

* An offset to an untyped vector, whose fields are of the given bit-width.

* An offset to untyped map, whose fields are of the given bit-width

* An offset to a typed vector, whose fields are of the given bit-width. The element type is specified in the top bits.

* An offset to a fixed typed vector of 2, 3, or 4 elements. These sizes of typed vectors are common enough that saving the size field is worth it (reducing a "Vec3" from 16 to 12 bytes).

* An offset to a string of bytes. Has both a length field (of the given bit-width), and 0 termination for c_str compatibility. Contents can be any binary, but if it is intended to be text, must be UTF-8.

An element type (2 bits) can only be scalar: signed/unsigned int, or float (all of indicated bit-width).

A vector always starts with a length field in the chosen number of bits, followed by the array, with each element in the chosen number of bits. If the vector is untyped (each element has its own type), then additionally length number of type bytes follow after the vector data.

A map is the same as a vector, but is pre-fixed by an additional offset, which points to a vector of strings which are the field names (the type of this must always be "typed vector of string"). The two vectors must be equal size. The field names being accessed over an offset means that multiple maps that happen to have the same field names can share this vector (!) for very compact storage of maps. Vectors of field names (and thus also the values) must be stored in sorted order, allowing in-place binary search lookup if needed. Code that works on vectors can work on maps (and field names) generically, and maps can be treated as vectors.

A string is different from a "vector of int8_t" because vectors have size fields with bit-widths the same as their contents, so that means it can store at most 256 elements before they'd need to become a "vector of int16_t" instead. Additionally they're null-terminated.

# Interop

It should be easy to convert any JSON to this format, and back. Conversion back and forth to FlatBuffers should also be possible.

# API

The advantage of the above design is its possible compactness, by allowing the encoder to choose a bit-width for fields, and inline vs offset storage. The disadvantage is that there's many possible ways an integer may be encoded.

As such, there is a simple API that allows me to get a field as an integer of my desired size, regardless of how it is stored.

Generic values are represented using an offset and a type. You can ask various things of the type (is this thing a vector?) and/or simply access a field as a particular type, which either succeeds or asserts.

# Project

This could be its own serialization library, but rather I think there are many advantages for this to be part of FlatBuffers:

* It can reuse a lot of functionality in flatbuffers.h, the existing JSON parser etc.

* We can offer conversion between the formats (and JSON) out of the box.

* Less friction for people that need both kinds of serialization, or may want to store a blob of one inside the other etc.

Implementation complexity of the above format is very low compared to the existing FlatBuffers (no schemas, no compiler, no generated code), and can be as small as a single header.

# Wire format

Much like FlatBuffers, elements of this buffer are written as little-endian, aligned to their own size.

It is constructed backwards, much like FlatBuffers, allowing the root piece of data to always be at the start of the buffer, and accessed without an offset. Unlike FlatBuffers, any value can be the root, including a lone number.

It is constructed depth-first, much like FlatBuffers. Besides that allowing the buffer to be constructed with few temporary data structures, it has the additional advantage that in most cases, the builder code will choose optimal bit-widths for you automatically (i.e. it checks if all offsets or integers fit in 8 or 16 bits, and then uses that instead). It may additionally do automatic string pooling or vector-of-string pooling upon request.

The file starts with the type of the root (these may not be any inline types), followed 8bit offset to it (needed because of possible alignment, and to allow for a file_identifier like FlatBuffers). For example, the smallest possible buffer is:

uint8_t INT_AT_OFFSET_8, offset_to_int
// Start of int:
uint8_t 1

I.e. 3 bytes total, representing the integer "1".

Example encoding of this piece of JSON:

{ "vec": [ 1.41421, 2.0, 3.14159 ], "hp": 1000 }

uint8_t UNTYPED_MAP_16, offset_to_root_map
// Start of root map
uint16_t offset_to_key_vector
uint16_t 2 // size
uint16_t offset_to_float_vector
uint16_t 1000
uint8_t FIXED_TYPED_VECTOR_32_3 | ELEMENT_FLOAT, INT_16  // type table
// Start of float vector
float32_t 1.41421, 2.0, 3.14159
// Start of key vector (these can be shared between multiple objects!)
uint8_t 2 // size
uint8_t offset_to_vec_string
uint8_t offset_to_hp_string
// Start of "vec" string (can also be shared, independently of key table!)
uint8_t 3, 'v', 'e', 'c', '\0'
// Start of "hp" string.
uint8_t 2, 'h', 'p', '\0'

36 bytes, about 30% smaller than the JSON while retaining all its self-describing information, and efficiently accessible without parsing or allocating.
But the savings could be much bigger for larger data sets, with full key vector sharing employed.

