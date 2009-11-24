#include <xml_namespace.h>

VALUE cNokogiriXmlNamespace ;

/*
 * call-seq:
 *  prefix
 *
 * Get the prefix for this namespace.  Returns +nil+ if there is no prefix.
 */
static VALUE prefix(VALUE self, SEL sel)
{
  xmlNsPtr ns;
  xmlDocPtr doc;

  Data_Get_Struct(self, xmlNs, ns);
  if(!ns->prefix) return Qnil;

  Data_Get_Struct(rb_ivar_get(self, "@document"), xmlDoc, doc);

  return NOKOGIRI_STR_NEW2(ns->prefix);
}

/*
 * call-seq:
 *  href
 *
 * Get the href for this namespace
 */
static VALUE href(VALUE self, SEL sel)
{
  xmlNsPtr ns;
  xmlDocPtr doc;

  Data_Get_Struct(self, xmlNs, ns);
  if(!ns->href) return Qnil;

  Data_Get_Struct(rb_ivar_get(self, "@document"), xmlDoc, doc);

  return NOKOGIRI_STR_NEW2(ns->href);
}

VALUE Nokogiri_wrap_xml_namespace(xmlDocPtr doc, xmlNsPtr node)
{
  assert(doc->_private);

  if(node->_private)
    return (VALUE)node->_private;

  VALUE ns = Data_Wrap_Struct(cNokogiriXmlNamespace, 0, 0, node);

  VALUE document = DOC_RUBY_OBJECT(doc);

  VALUE node_cache = rb_ivar_get(document, "@node_cache");
  rb_ary_push(node_cache, ns);

  rb_ivar_set(ns, "@document", DOC_RUBY_OBJECT(doc));

  node->_private = (void *)ns;

  return ns;
}

VALUE Nokogiri_wrap_xml_namespace2(VALUE document, xmlNsPtr node)
{
  xmlDocPtr doc;
  Data_Get_Struct(document, xmlDoc, doc) ;
  return Nokogiri_wrap_xml_namespace(doc, node);
}


void init_xml_namespace()
{
  VALUE nokogiri  = rb_define_module("Nokogiri");
  VALUE xml       = rb_define_module_under(nokogiri, "XML");
  VALUE klass     = rb_define_class_under(xml, "Namespace", rb_cObject);

  cNokogiriXmlNamespace = klass;

  rb_objc_define_method(klass, "prefix", prefix, 0);
  rb_objc_define_method(klass, "href", href, 0);
}
