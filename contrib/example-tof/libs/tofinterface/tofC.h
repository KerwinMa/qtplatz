// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.1.9
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_codegen.cpp:152

#ifndef _TAO_IDL_TOFC_F7H2GJ_H_
#define _TAO_IDL_TOFC_F7H2GJ_H_

#include /**/ "ace/pre.h"


#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/String_Manager_T.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Basic_Arguments.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/Fixed_Size_Argument_T.h"
#include "tao/Var_Size_Argument_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/UB_String_Arguments.h"
#include /**/ "tao/Version.h"
#include /**/ "tao/Versioned_Namespace.h"

#include "adinterface/brokerclientC.h"
#include "adinterface/global_constantsC.h"
#include "adinterface/instrumentC.h"
#include "methodC.h"
#include "sioC.h"

#if TAO_MAJOR_VERSION != 2 || TAO_MINOR_VERSION != 1 || TAO_BETA_VERSION != 9
#error This file should be regenerated with TAO_IDL
#endif

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO 

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_root\root_ch.cpp:160
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<typename T> class Narrow_Utils;
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:38

namespace TOFConstants
{

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

  const CORBA::ULong ClassID_AnalyzerDeviceData = 268566529U;

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

  const CORBA::ULong ClassID_ControlMethod = 268566530U;

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

  const CORBA::ULong ClassID_IonSourceMethod = 268566531U;

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

  const CORBA::ULong ClassID_ProfileData = 268566532U;

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:67

} // module TOFConstants

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:38

namespace TOF
{

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:38

  namespace Constants
  {

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

    const CORBA::ULong ClassID_AnalyzerDeviceData = 268566529U;

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

    const CORBA::ULong ClassID_MSMethod = 268566530U;

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

    const CORBA::ULong ClassID_IonSourceMethod = 268566531U;

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_constant\constant_ch.cpp:38

    const CORBA::ULong ClassID_ProfileData = 268566532U;
  
  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:67
  
  } // module TOF::Constants

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_interface.cpp:750

#if !defined (_TOF_SESSION__VAR_OUT_CH_)
#define _TOF_SESSION__VAR_OUT_CH_

  class Session;
  typedef Session *Session_ptr;

  typedef
    TAO_Objref_Var_T<
        Session
      >
    Session_var;
  
  typedef
    TAO_Objref_Out_T<
        Session
      >
    Session_out;

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_interface\interface_ch.cpp:43

  class  Session
    : public virtual ::Instrument::Session
  
  {
  public:
    friend class TAO::Narrow_Utils<Session>;

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    typedef Session_ptr _ptr_type;
    typedef Session_var _var_type;
    typedef Session_out _out_type;

    static void _tao_any_destructor (void *);

    // The static operations.
    static Session_ptr _duplicate (Session_ptr obj);

    static void _tao_release (Session_ptr obj);

    static Session_ptr _narrow (::CORBA::Object_ptr obj);
    static Session_ptr _unchecked_narrow (::CORBA::Object_ptr obj);
    static Session_ptr _nil (void);

    virtual void debug (
      const ::CORBA::WChar * text,
      const ::CORBA::WChar * key);

    virtual ::CORBA::Boolean setControlMethod (
      const ::TOF::ControlMethod & method,
      const char * hint);

    // TAO_IDL - Generated from
    // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_interface\interface_ch.cpp:140

    virtual ::CORBA::Boolean _is_a (const char *type_id);
    virtual const char* _interface_repository_id (void) const;
    virtual ::CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  
  protected:
    // Concrete interface only.
    Session (void);

    // Concrete non-local interface only.
    Session (
        ::IOP::IOR *ior,
        TAO_ORB_Core *orb_core);
    
    // Non-local interface only.
    Session (
        TAO_Stub *objref,
        ::CORBA::Boolean _tao_collocated = false,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0);

    virtual ~Session (void);
  
  private:
    // Private and unimplemented for concrete interfaces.
    Session (const Session &);

    void operator= (const Session &);
  };

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_typecode\typecode_decl.cpp:37

  extern  ::CORBA::TypeCode_ptr const _tc_Session;

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:67

} // module TOF

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:68

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  // TAO_IDL - Generated from
  // c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:147

#if !defined (_TOF_SESSION__ARG_TRAITS_)
#define _TOF_SESSION__ARG_TRAITS_

  template<>
  class  Arg_Traits< ::TOF::Session>
    : public
        Object_Arg_Traits_T<
            ::TOF::Session_ptr,
            ::TOF::Session_var,
            ::TOF::Session_out,
            TAO::Objref_Traits<TOF::Session>,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

#endif /* end #if !defined */
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_traits.cpp:62

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Traits specializations.
namespace TAO
{

#if !defined (_TOF_SESSION__TRAITS_)
#define _TOF_SESSION__TRAITS_

  template<>
  struct  Objref_Traits< ::TOF::Session>
  {
    static ::TOF::Session_ptr duplicate (
        ::TOF::Session_ptr p);
    static void release (
        ::TOF::Session_ptr p);
    static ::TOF::Session_ptr nil (void);
    static ::CORBA::Boolean marshal (
        const ::TOF::Session_ptr p,
        TAO_OutputCDR & cdr);
  };

#endif /* end #if !defined */
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_interface\any_op_ch.cpp:44



#if defined (ACE_ANY_OPS_USE_NAMESPACE)

namespace TOF
{
   void operator<<= ( ::CORBA::Any &, Session_ptr); // copying
   void operator<<= ( ::CORBA::Any &, Session_ptr *); // non-copying
   ::CORBA::Boolean operator>>= (const ::CORBA::Any &, Session_ptr &);
}

#else


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 void operator<<= (::CORBA::Any &, TOF::Session_ptr); // copying
 void operator<<= (::CORBA::Any &, TOF::Session_ptr *); // non-copying
 ::CORBA::Boolean operator>>= (const ::CORBA::Any &, TOF::Session_ptr &);
TAO_END_VERSIONED_NAMESPACE_DECL



#endif

// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_visitor_interface\cdr_op_ch.cpp:44

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const TOF::Session_ptr );
 ::CORBA::Boolean operator>> (TAO_InputCDR &, TOF::Session_ptr &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// c:\users\toshi\src\vc11\ace_wrappers\tao\tao_idl\be\be_codegen.cpp:1703
#if defined (__ACE_INLINE__)
#include "tofC.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* ifndef */

