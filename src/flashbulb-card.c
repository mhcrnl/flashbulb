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

typedef struct _FlashbulbCardXmlParseData FlashbulbCardXmlParseData;
/* TODO: possibly make this a full-fledged class */
struct _FlashbulbCardXmlParseData
{
	FlashbulbCard *card;
	gboolean       question_encountered;
	gboolean       answer_encountered;
};

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
flashbulb_card_init (FlashbulbCard *self) {}

static void
flashbulb_card_finalize (GObject *object)
{
	FlashbulbCardPrivate *priv;

	priv = FLASHBULB_CARD_PRIVATE (FLASHBULB_CARD (object));

	g_free (priv->question);
	g_free (priv->answer);

	G_OBJECT_CLASS (flashbulb_card_parent_class)->finalize (object);
}

static void
flashbulb_card_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
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
flashbulb_card_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
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

	g_object_class_install_property (
		object_class, PROP_QUESTION,
		g_param_spec_string ("question",
		                     "q",
		                     "The card's question.",
		                     "",
		                     G_PARAM_READWRITE | G_PARAM_STATIC_NAME
		                     | G_PARAM_STATIC_NICK
		                     | G_PARAM_STATIC_BLURB));

	g_object_class_install_property (
		object_class, PROP_ANSWER,
		g_param_spec_string ("answer",
		                     "a",
		                     "The answer to the card's question.",
		                     "",
		                     G_PARAM_READWRITE | G_PARAM_STATIC_NAME
		                     | G_PARAM_STATIC_NICK
		                     | G_PARAM_STATIC_BLURB));
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
flashbulb_card_is_correct (const FlashbulbCard *self, const gchar *answer)
{
	gchar   *self_answer;
	gboolean ret;

	/* NOTE: this casts away the const, but get_answer() is declared
	   const anyway */
	g_object_get ((gpointer) self, "answer", &self_answer, NULL);
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

static FlashbulbCardXmlParseData*
flashbulb_card_xml_parse_data_new (void)
{
	FlashbulbCardXmlParseData *ret;

	ret = g_malloc (sizeof (FlashbulbCardXmlParseData));

	ret->card = g_object_new (FLASHBULB_TYPE_CARD, NULL);
	ret->question_encountered = FALSE;
	ret->answer_encountered = FALSE;

	return ret;
}

static gboolean
flashbulb_card_is_valid_question_element (const GSList *element_stack)
{
	return ((g_slist_length ((GSList *) element_stack) == 2) &&
	        !g_strcmp0 (element_stack->next->data, "flashcard") &&
	        !g_strcmp0 (element_stack->data, "question"));
}

static gboolean
flashbulb_card_is_valid_answer_element (const GSList *element_stack)
{
	return ((g_slist_length ((GSList *) element_stack) == 2) &&
	        !g_strcmp0 (element_stack->next->data, "flashcard") &&
	        !g_strcmp0 (element_stack->data, "answer"));
}

static void
flashbulb_card_from_xml_start_element (GMarkupParseContext *context,
                                       const gchar         *element_name,
                                       const gchar        **attribute_names,
                                       const gchar        **attribute_values,
                                       gpointer             user_data,
                                       GError             **error)
{
	const GSList              *element_stack;
	FlashbulbCardXmlParseData *data;

	data = (FlashbulbCardXmlParseData *) user_data;
	element_stack = g_markup_parse_context_get_element_stack (context);
	if (g_slist_length ((GSList *) element_stack) == 1 &&
	    g_strcmp0 (element_stack->data, "flashcard")) {
		/* TODO: set error */
	} else if (flashbulb_card_is_valid_question_element (element_stack)) {
		if (data->question_encountered) {
			/* TODO: set error */
		} else {
			data->question_encountered = TRUE;
		}
	} else if (flashbulb_card_is_valid_answer_element (element_stack)) {
		if (data->answer_encountered) {
			/* TODO: set error */
		} else {
			data->question_encountered = TRUE;
		}
	}
}

static void
flashbulb_card_from_xml_text (GMarkupParseContext *context,
                              const gchar         *text,
                              gsize                text_len,
                              gpointer             user_data,
                              GError             **error)
{
	const GSList              *element_stack;
	FlashbulbCardXmlParseData *data;

	/* TODO: if FlashbulbCardXmlParseData becomes a class, make
	   this use the "official" cast macro */
	data = (FlashbulbCardXmlParseData *) user_data;
	element_stack = g_markup_parse_context_get_element_stack (context);

	if (flashbulb_card_is_valid_question_element (element_stack)) {
		g_object_set (data->card, "question", text, NULL);
	} else if (flashbulb_card_is_valid_answer_element (element_stack)) {
		g_object_set (data->card, "answer", text, NULL);
	}
}

FlashbulbCard *
flashbulb_card_from_xml (const gchar *xml_text, gssize text_len)
{
	FlashbulbCardXmlParseData *parse_data;
	GMarkupParseContext       *context;
	FlashbulbCard             *ret;
	GError                    *error;
	static const GMarkupParser flashbulb_card_xml_parser = {
		.start_element = flashbulb_card_from_xml_start_element,
		.end_element = NULL,
		.text = flashbulb_card_from_xml_text,
		.passthrough = NULL,
		.error = NULL
	};

	parse_data = flashbulb_card_xml_parse_data_new ();
	context = g_markup_parse_context_new (&flashbulb_card_xml_parser,
	                                      0,
	                                      parse_data,
	                                      NULL);
	g_markup_parse_context_parse (context, xml_text, text_len, &error);
	g_markup_parse_context_free (context);

	ret = parse_data->card;
	/* TODO: possibly add a function for freeing parse data */
	g_free (parse_data);
	return ret;
}
