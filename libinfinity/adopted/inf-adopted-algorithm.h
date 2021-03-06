/* libinfinity - a GObject-based infinote implementation
 * Copyright (C) 2007-2014 Armin Burgmeier <armin@arbur.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef __INF_ADOPTED_ALGORITHM_H__
#define __INF_ADOPTED_ALGORITHM_H__

#include <libinfinity/adopted/inf-adopted-request-log.h>
#include <libinfinity/adopted/inf-adopted-user.h>
#include <libinfinity/common/inf-user-table.h>
#include <libinfinity/common/inf-buffer.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define INF_ADOPTED_TYPE_ALGORITHM                 (inf_adopted_algorithm_get_type())
#define INF_ADOPTED_ALGORITHM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), INF_ADOPTED_TYPE_ALGORITHM, InfAdoptedAlgorithm))
#define INF_ADOPTED_ALGORITHM_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST((klass), INF_ADOPTED_TYPE_ALGORITHM, InfAdoptedAlgorithmClass))
#define INF_ADOPTED_IS_ALGORITHM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), INF_ADOPTED_TYPE_ALGORITHM))
#define INF_ADOPTED_IS_ALGORITHM_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE((klass), INF_ADOPTED_TYPE_ALGORITHM))
#define INF_ADOPTED_ALGORITHM_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS((obj), INF_ADOPTED_TYPE_ALGORITHM, InfAdoptedAlgorithmClass))

typedef struct _InfAdoptedAlgorithm InfAdoptedAlgorithm;
typedef struct _InfAdoptedAlgorithmClass InfAdoptedAlgorithmClass;

/**
 * InfAdoptedAlgorithmError:
 * @INF_ADOPTED_ALGORITHM_ERROR_NO_UNDO: An undo request was about to be
 * executed but there is no operation to undo.
 * @INF_ADOPTED_ALGORITHM_ERROR_NO_REDO: A redo request was about to be
 * executed but there is no operation to redo.
 * @INF_ADOPTED_ALGORITHM_ERROR_FAILED: No further specified error code.
 *
 * Error codes for #InfAdoptedAlgorithm. These can occur when invalid requests
 * are passed to inf_adopted_algorithm_execute_request().
 */
typedef enum _InfAdoptedAlgorithmError {
  INF_ADOPTED_ALGORITHM_ERROR_NO_UNDO,
  INF_ADOPTED_ALGORITHM_ERROR_NO_REDO,
  
  INF_ADOPTED_ALGORITHM_ERROR_FAILED
} InfAdoptedAlgorithmError;

/**
 * InfAdoptedAlgorithmClass:
 * @can_undo_changed: Default signal handler for the
 * #InfAdoptedAlgorithm::can_undo_changed signal.
 * @can_redo_changed: Default signal handler for the
 * #InfAdoptedAlgorithm::can_redo_changed signal.
 * @execute_request: Default signal handler for the
 * #InfAdoptedAlgorithm::execute_request signal.
 * @apply_request: Default signal handler for the
 * #InfAdoptedAlgorithm::apply_request signal.
 *
 * Signals for the #InfAdoptedAlgorithm class.
 */
struct _InfAdoptedAlgorithmClass {
  /*< private >*/
  GObjectClass parent_class;

  /* Signals */

  /*< public >*/
  void(*can_undo_changed)(InfAdoptedAlgorithm* algorithm,
                          InfAdoptedUser* user,
                          gboolean can_undo);

  void(*can_redo_changed)(InfAdoptedAlgorithm* algorithm,
                          InfAdoptedUser* user,
                          gboolean can_redo);

  void(*begin_execute_request)(InfAdoptedAlgorithm* algorithm,
                               InfAdoptedUser* user,
                               InfAdoptedRequest* request);

  void(*end_execute_request)(InfAdoptedAlgorithm* algorithm,
                             InfAdoptedUser* user,
                             InfAdoptedRequest* request,
                             InfAdoptedRequest* translated_request,
                             const GError* error);
};

/**
 * InfAdoptedAlgorithm:
 *
 * #InfAdoptedAlgorithm is an opaque data type. You should only access it via
 * the public API functions.
 */
struct _InfAdoptedAlgorithm {
  /*< private >*/
  GObject parent;
  gpointer priv;
};

GType
inf_adopted_algorithm_get_type(void) G_GNUC_CONST;

InfAdoptedAlgorithm*
inf_adopted_algorithm_new(InfUserTable* user_table,
                          InfBuffer* buffer);

InfAdoptedAlgorithm*
inf_adopted_algorithm_new_full(InfUserTable* user_table,
                               InfBuffer* buffer,
                               guint max_total_log_size);

InfAdoptedStateVector*
inf_adopted_algorithm_get_current(InfAdoptedAlgorithm* algorithm);

InfAdoptedRequest*
inf_adopted_algorithm_get_execute_request(InfAdoptedAlgorithm* algorithm);

InfAdoptedRequest*
inf_adopted_algorithm_generate_request(InfAdoptedAlgorithm* algorithm,
                                       InfAdoptedRequestType type,
                                       InfAdoptedUser* user,
                                       InfAdoptedOperation* operation);

InfAdoptedRequest*
inf_adopted_algorithm_translate_request(InfAdoptedAlgorithm* algorithm,
                                        InfAdoptedRequest* request,
                                        InfAdoptedStateVector* to);

gboolean
inf_adopted_algorithm_execute_request(InfAdoptedAlgorithm* algorithm,
                                      InfAdoptedRequest* request,
                                      gboolean apply,
                                      GError** error);

void
inf_adopted_algorithm_cleanup(InfAdoptedAlgorithm* algorithm);

gboolean
inf_adopted_algorithm_can_undo(InfAdoptedAlgorithm* algorithm,
                               InfAdoptedUser* user);

gboolean
inf_adopted_algorithm_can_redo(InfAdoptedAlgorithm* algorithm,
                               InfAdoptedUser* user);

G_END_DECLS

#endif /* __INF_ADOPTED_ALGORITHM_H__ */

/* vim:set et sw=2 ts=2: */
