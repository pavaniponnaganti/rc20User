/**
 * @file dbSchemaCRC.h
 * @brief Define for the schema checksum value. This is the CRC32 value of all
 *        of the generated schema files (excluding this file). It is contained
 *        within a seperate header file so that it does not itself alter the
 *        checksum value of the files.
 * 
 * The DB_CRC32 define in this file is automatically updated and should not
 * be altered manually.
 */
#ifndef _DBSCHEMACRC_H_
#define _DBSCHEMACRC_H_

/** Define for the schema checksum value. */
#define DB_CRC32 0x

#endif // #ifndef _DBSCHEMACRC_H_
