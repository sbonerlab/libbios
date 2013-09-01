/**
 * \file types.h
 * \author Macros adapted by David Z. Chen from Linux 2.6 code
 */
#ifndef TYPES_H
#define TYPES_H

#define typeof __typeof__

/**
 * Get offset of a member
 */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * Note: this variant of container_of does not perform type checking to make sure
 * that the type of ptr matches that of the type of the struct's member it is
 * supposed to be
 * @param[in] ptr: the pointer to the member
 * @param[in] type: the type of the container struct this is embedded in
 * @param[in] member: the name of the member within the struct
 */
#define container_of(ptr, type, member) \
	(type *)( (char *)ptr - offsetof(type, member) )


#endif
