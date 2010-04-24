/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * flashbulb
 * Copyright (C) Nicholas Coltharp 2010 <ngc1@rice.edu>
 * 
 * flashbulb is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * flashbulb is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FLASHBULB_CARD_H__
#define __FLASHBULB_CARD_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define FLASHBULB_TYPE_CARD             (flashbulb_card_get_type ())
#define FLASHBULB_CARD(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FLASHBULB_TYPE_CARD, FlashbulbCard))
#define FLASHBULB_CARD_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FLASHBULB_TYPE_CARD, FlashbulbCardClass))
#define FLASHBULB_IS_CARD(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FLASHBULB_TYPE_CARD))
#define FLASHBULB_IS_CARD_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FLASHBULB_TYPE_CARD))
#define FLASHBULB_CARD_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FLASHBULB_TYPE_CARD, FlashbulbCardClass))

typedef struct _FlashbulbCardClass FlashbulbCardClass;
typedef struct _FlashbulbCard FlashbulbCard;

struct _FlashbulbCardClass
{
	GObjectClass parent_class;

	/* Virtual methods */
	void (* get_question)   (const FlashbulbCard *self,
	                         GValue *value);
	void (* get_answer)     (const FlashbulbCard *self,
	                         GValue *value);
	void (* set_question)   (FlashbulbCard *self,
	                         const GValue *value);
	void (* set_answer)     (FlashbulbCard *self,
	                         const GValue *value);
	gboolean (* is_correct) (FlashbulbCard *self,
	                         const gchar *answer);
	gboolean (* test)       (FlashbulbCard *self,
	                         const gchar *answer);
	gboolean (* succeed)    (FlashbulbCard *self,
	                         const gchar *answer);
	gboolean (* fail)       (FlashbulbCard *self,
	                         const gchar *answer);
};

struct _FlashbulbCard
{
	GObject parent_instance;
};

GType    flashbulb_card_get_type     (void) G_GNUC_CONST;
void     flashbulb_card_get_question (const FlashbulbCard *self,
                                      GValue              *value);
void     flashbulb_card_get_answer   (const FlashbulbCard *self,
                                      GValue              *value);
void     flashbulb_card_set_question (FlashbulbCard       *self,
                                      const GValue        *value);
void     flashbulb_card_set_answer   (FlashbulbCard       *self,
                                      const GValue        *value);
gboolean flashbulb_card_is_correct   (FlashbulbCard       *self,
                                      const gchar         *answer) G_GNUC_CONST;
gboolean flashbulb_card_test         (FlashbulbCard       *self,
                                      const gchar         *answer);
gboolean flashbulb_card_succeed      (FlashbulbCard       *self,
                                      const gchar         *answer);
gboolean flashbulb_card_fail         (FlashbulbCard       *self,
                                      const gchar         *answer);

G_END_DECLS

#endif /* __FLASHBULB_CARD_H__ */
