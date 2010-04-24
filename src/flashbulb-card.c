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

#include "flashbulb-card.h"

#include <glib.h>
#include <glib-object.h>

typedef struct _FlashbulbCardPrivate FlashbulbCardPrivate;
struct _FlashbulbCardPrivate
{
	gchar *question;
	gchar *answer;
};

#define FLASHBULB_CARD_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), FLASHBULB_TYPE_CARD, FlashbulbCardPrivate))

enum {
	PROP_0,

	PROP_QUESTION,
	PROP_ANSWER
};

G_DEFINE_TYPE (FlashbulbCard, flashbulb_card, G_TYPE_OBJECT);

static void
flashbulb_card_init (FlashbulbCard *object)
{
	/* TODO: Add initialization code here */
}

static void
flashbulb_card_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (flashbulb_card_parent_class)->finalize (object);
}

static void
flashbulb_card_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	FlashbulbCard *self;

	self = FLASHBULB_CARD (object);

	switch (prop_id) {
	case PROP_QUESTION:
		FLASHBULB_CARD_GET_CLASS (self)->set_question (self, value);
		break;
	case PROP_ANSWER:
		FLASHBULB_CARD_GET_CLASS (self)->set_answer (self, value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
flashbulb_card_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	FlashbulbCard *self;

	self = FLASHBULB_CARD (object);

	switch (prop_id) {
	case PROP_QUESTION:
		FLASHBULB_CARD_GET_CLASS (self)->get_question (self, value);
		break;
	case PROP_ANSWER:
		FLASHBULB_CARD_GET_CLASS (self)->get_answer (self, value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
flashbulb_card_class_init (FlashbulbCardClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/* GObjectClass* parent_class = G_OBJECT_CLASS (klass); */

	g_type_class_add_private (klass, sizeof (FlashbulbCardPrivate));

	object_class->finalize = flashbulb_card_finalize;
	object_class->set_property = flashbulb_card_set_property;
	object_class->get_property = flashbulb_card_get_property;

	klass->get_question = flashbulb_card_get_question;
	klass->get_answer = flashbulb_card_get_answer;
	klass->set_question = flashbulb_card_set_question;
	klass->set_answer = flashbulb_card_set_answer;
	klass->is_correct = flashbulb_card_is_correct;
	klass->test = flashbulb_card_test;
	klass->succeed = flashbulb_card_succeed;
	klass->fail = flashbulb_card_fail;

	g_object_class_install_property (object_class,
	                                 PROP_QUESTION,
	                                 g_param_spec_string ("question",
	                                                      "q",
	                                                      "The card's question.",
	                                                      "",
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB));

	g_object_class_install_property (object_class,
	                                 PROP_ANSWER,
	                                 g_param_spec_string ("answer",
	                                                      "a",
	                                                      "The answer to the card's question.",
	                                                      "",
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB));
}

void
flashbulb_card_get_question (const FlashbulbCard *self, GValue *value)
{
	g_value_set_string (value, FLASHBULB_CARD_PRIVATE (self)->question);
}

void
flashbulb_card_get_answer (const FlashbulbCard *self, GValue *value)
{
	g_value_set_string (value, FLASHBULB_CARD_PRIVATE (self)->answer);
}

void
flashbulb_card_set_question (FlashbulbCard *self, const GValue *value)
{
	FlashbulbCardPrivate *priv;

	priv = FLASHBULB_CARD_PRIVATE (self);
	g_free (priv->question);
	priv->question = g_value_dup_string (value);
}

void
flashbulb_card_set_answer (FlashbulbCard *self, const GValue *value)
{
	FlashbulbCardPrivate *priv;

	priv = FLASHBULB_CARD_PRIVATE (self);
	g_free (priv->answer);
	priv->answer = g_value_dup_string (value);
}

gboolean
flashbulb_card_is_correct (FlashbulbCard *self, const gchar *answer)
{
	gchar *self_answer;
	gboolean ret;

	g_object_get (self, "answer", &self_answer, NULL);
	ret = !g_strcmp0 (answer, self_answer);
	g_free (self_answer);

	return ret;
}

gboolean
flashbulb_card_test (FlashbulbCard *self, const gchar *answer)
{
	FlashbulbCardClass *klass;

	klass = FLASHBULB_CARD_GET_CLASS (self);
	return (flashbulb_card_is_correct (self, answer) ?
	        klass->succeed (self, answer) : klass->fail (self, answer));
}

gboolean
flashbulb_card_succeed (FlashbulbCard *self, const gchar *answer)
{
	return TRUE;
}

gboolean
flashbulb_card_fail (FlashbulbCard *self, const gchar *answer)
{
	return FALSE;
}
