/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace _{
    namespace policy_list_impl{
      //determine if two policies are identical
      template <template <class> class, template <class> class> struct is_same_policy : std::false_type{};
      template <template <class> class _PolicyT> struct is_same_policy<_PolicyT, _PolicyT> : std::true_type{};


      //append a policy_list to an existing policy_list
      template <class _SourceList, class _AppendList> struct append_list;
      template <template <class> class ... _SourceListTs, template <class> class ... _AppendListTs>
      struct append_list<policy_list<_SourceListTs...>, policy_list<_AppendListTs...>>{
        using type = policy_list<_SourceListTs..., _AppendListTs...>;
      };

      //see if a policy exists in a parameter pack
      template <template <class> class _TargetT, template <class> class ... _Policies> struct exists;
      template <template <class> class _TargetT> struct exists<_TargetT> : std::false_type{};

      template <template <class> class _TargetT, template <class> class ... _TailT>
      struct exists<_TargetT, _TargetT, _TailT...> : std::true_type{};

      template <template <class> class _TargetT, template <class> class _HeadT, template <class> class ... _TailT>
      struct exists<_TargetT, _HeadT, _TailT...> : exists<_TargetT, _TailT...>{};

      //remove all occurrences of a policy from a parameter pack
      template <template <class> class _TargetT, template <class> class ... _Policies> struct remove_all;
      template <template <class> class _TargetT> struct remove_all<_TargetT>{ using type = policy_list<>; };


      template <bool, template <class> class _TargetT, template <class> class ... _Policies> struct remove_all_b;

      template <template <class> class _TargetT, template <class> class _HeadT, template <class> class ... _TailT>
      struct remove_all_b<true, _TargetT, _HeadT, _TailT...>{
        using type = typename remove_all<_TargetT, _TailT...>::type;
      };

      template <template <class> class _TargetT, template <class> class _HeadT, template <class> class ... _TailT>
      struct remove_all_b<false, _TargetT, _HeadT, _TailT...>{
        using tmp_tail = typename remove_all<_TargetT, _TailT...>::type;
        using type = typename append_list<policy_list<_HeadT>, tmp_tail>::type;
      };


      template <template <class> class _TargetT, template <class> class _HeadT, template <class> class ... _TailT>
      struct remove_all<_TargetT, _HeadT, _TailT...> : remove_all_b< is_same_policy<_TargetT, _HeadT>::value, _TargetT, _HeadT, _TailT...>{};


      //remove all occurrences of a policy from a policy list
      template <template <class> class _TargetT, class _PolicyListT> struct remove_all_list;
      template <template <class> class _TargetT, template <class> class ... _Policies>
      struct remove_all_list<_TargetT, policy_list<_Policies...>>{ using type = typename remove_all<_TargetT, _Policies...>::type; };


      //reverse order of policy parameter pack
      template <template <class> class ... _Policies> struct reverse;
      template <> struct reverse<>{ using type = policy_list<>; };

      template <template <class> class _HeadT, template <class> class ... _TailT>
      struct reverse<_HeadT, _TailT...>{
        using reversed_tail = typename reverse<_TailT...>::type;
        using type = typename append_list<reversed_tail, policy_list<_HeadT>>::type;
      };

      //reverse policy list
      template <class _PolicyListT> struct reverse_list;
      template <template <class> class ... _Policies>
      struct reverse_list<policy_list<_Policies...>>{ using type = typename reverse < _Policies...>::type; };

      //undup list
      template <class> struct undup_list;
      template <> struct undup_list<policy_list<>>{ using type = policy_list<>; };
      template <template <class> class _HeadT, template <class> class ... _TailT>
      struct undup_list<policy_list<_HeadT, _TailT...>>{
        using unduped_tail = typename undup_list<policy_list<_TailT...>>::type;
        using unduped_head = typename remove_all_list<_HeadT, unduped_tail>::type;
        using type = typename append_list<policy_list<_HeadT>, unduped_head>::type;
      };

    }


    template <> struct policy_list<>{
      template<template <class> class ... _AppendT>
      using append = policy_list<_AppendT...>;

      template <class _AppendT>
      using append_list = _AppendT;

      template <template <class> class _TargetT>
      using exists = std::false_type;

      template <template <class> class _TargetT>
      using remove = policy_list<>;

      using reverse = policy_list<>;

      using dedup = policy_list<>;

    };

    template <template <class> class _HeadT, template <class> class ... _TailT>
    struct policy_list<_HeadT, _TailT...> : policy_list<_TailT...>{

      template<template <class> class ... _AppendT>
      using append = policy_list<_HeadT, _TailT..., _AppendT...>;

      template <class _AppendT>
      using append_list = typename _::policy_list_impl::append_list<policy_list<_HeadT, _TailT...>, _AppendT>::type;

      template <template <class> class _TargetT>
      using exists = typename _::policy_list_impl::exists<_TargetT, _HeadT, _TailT...>::type;


      using reverse = typename _::policy_list_impl::reverse<_HeadT, _TailT...>::type;

      using dedup = typename _::policy_list_impl::undup_list<policy_list<_HeadT, _TailT...>>::type;
    };


    template <class> struct policy_collector;
    template <> struct policy_collector<policy_list<>>{ using type = policy_list<>; };


    template <template <class> class _HeadT, template <class> class ... _TailT>
    struct policy_collector<policy_list<_HeadT, _TailT...>>{
      using head_reqs = typename policy_traits<_HeadT>::requires;
      using tail_list = typename policy_list<_TailT...>::template append_list<head_reqs>;
      using full_reqs = typename policy_collector<tail_list>::type;
      using type = typename policy_list<_HeadT>::template append_list<full_reqs>;
    };


    template <class _ImplT, class _PolicyListT> struct construct_hierarchy;
    template <class _ImplT> struct construct_hierarchy<_ImplT, policy_list<>>{ using type = wtf::window; };
    template <class _ImplT, template <class> class _HeadT, template <class> class ... _TailT>
    struct construct_hierarchy<_ImplT, policy_list<_HeadT, _TailT...>>{
      using tail_hierarchy = typename construct_hierarchy<_ImplT, policy_list<_TailT...>>::type;
      using type = _HeadT<tail_hierarchy>;
    };

    template <class _ImplT, template <class> class ... _Policies> struct normalized_policies{
      using collected_policies = typename policy_collector<policy_list<_Policies...>>::type;
      using reversed_policies = typename collected_policies::reverse;
      using unique_policies = typename reversed_policies::dedup;
      using unique_ordered_policies = typename unique_policies::reverse;
      using type = typename construct_hierarchy<_ImplT, unique_ordered_policies>::type;
    };

  }
}
