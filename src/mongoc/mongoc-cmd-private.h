/*
 * Copyright 2017 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * Internal struct to represent a command we will send to the server - command
 * parameters are collected in a mongoc_cmd_parts_t until we know the server's
 * wire version and whether it is mongos, then we collect the parts into a
 * mongoc_cmd_t, and gather that into a mongoc_rpc_t.
 */

#ifndef MONGOC_CMD_PRIVATE_H
#define MONGOC_CMD_PRIVATE_H

#if !defined(MONGOC_COMPILATION)
#error "Only <mongoc.h> can be included directly."
#endif

#include <bson.h>

#include "mongoc-server-stream-private.h"
#include "mongoc-read-prefs.h"
#include "mongoc.h"

BSON_BEGIN_DECLS

typedef struct _mongoc_cmd_t {
   const char *db_name;
   mongoc_query_flags_t query_flags;
   const bson_t *command;
   const char *command_name;
   const uint8_t *payload;
   int32_t payload_size;
   const char *payload_identifier;
   const mongoc_server_stream_t *server_stream;
   int64_t operation_id;
} mongoc_cmd_t;


typedef struct _mongoc_cmd_parts_t {
   mongoc_cmd_t assembled;
   mongoc_query_flags_t user_query_flags;
   const bson_t *body;
   bson_t extra;
   const mongoc_read_prefs_t *read_prefs;
   bson_t assembled_body;
   bool is_write_command;
   mongoc_client_session_t *session;
} mongoc_cmd_parts_t;


void
mongoc_cmd_parts_init (mongoc_cmd_parts_t *op,
                       const char *db_name,
                       mongoc_query_flags_t user_query_flags,
                       const bson_t *command_body);


bool
mongoc_cmd_parts_append_opts (mongoc_cmd_parts_t *parts,
                              bson_iter_t *iter,
                              int max_wire_version,
                              bson_error_t *error);

bool
mongoc_cmd_parts_assemble (mongoc_cmd_parts_t *parts,
                           const mongoc_server_stream_t *server_stream,
                           bson_error_t *error);

bool
mongoc_cmd_is_compressable (mongoc_cmd_t *cmd);

void
mongoc_cmd_parts_cleanup (mongoc_cmd_parts_t *op);

BSON_END_DECLS


#endif /* MONGOC_CMD_PRIVATE_H */
