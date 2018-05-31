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
      template <template <class> class _target_t, template <class> class ... _policy_ts> struct exists;
      template <template <class> class _target_t> struct exists<_target_t> : std::false_type{};

      template <template <class> class _target_t, template <class> class ... _tail_t>
      struct exists<_target_t, _target_t, _tail_t...> : std::true_type{};

      template <template <class> class _target_t, template <class> class _head_t, template <class> class ... _tail_t>
      struct exists<_target_t, _head_t, _tail_t...> : exists<_target_t, _tail_t...>{};

      //remove all occurrences of a policy from a parameter pack
      template <template <class> class _target_t, template <class> class ... _policy_ts> struct remove_all;
      template <template <class> class _target_t> struct remove_all<_target_t>{ using type = policy_list<>; };


      template <bool, template <class> class _target_t, template <class> class ... _policy_ts> struct remove_all_b;

      template <template <class> class _target_t, template <class> class _head_t, template <class> class ... _tail_t>
      struct remove_all_b<true, _target_t, _head_t, _tail_t...>{
        using type = typename remove_all<_target_t, _tail_t...>::type;
      };

      template <template <class> class _target_t, template <class> class _head_t, template <class> class ... _tail_t>
      struct remove_all_b<false, _target_t, _head_t, _tail_t...>{
        using tmp_tail = typename remove_all<_target_t, _tail_t...>::type;
        using type = typename append_list<policy_list<_head_t>, tmp_tail>::type;
      };


      template <template <class> class _target_t, template <class> class _head_t, template <class> class ... _tail_t>
      struct remove_all<_target_t, _head_t, _tail_t...> : remove_all_b< is_same_policy<_target_t, _head_t>::value, _target_t, _head_t, _tail_t...>{};


      //remove all occurrences of a policy from a policy list
      template <template <class> class _target_t, class _policy_list_t> struct remove_all_list;
      template <template <class> class _target_t, template <class> class ... _policy_ts>
      struct remove_all_list<_target_t, policy_list<_policy_ts...>>{ using type = typename remove_all<_target_t, _policy_ts...>::type; };


      //reverse order of policy parameter pack
      template <template <class> class ... _policy_ts> struct reverse;
      template <> struct reverse<>{ using type = policy_list<>; };

      template <template <class> class _head_t, template <class> class ... _tail_t>
      struct reverse<_head_t, _tail_t...>{
        using reversed_tail = typename reverse<_tail_t...>::type;
        using type = typename append_list<reversed_tail, policy_list<_head_t>>::type;
      };

      //reverse policy list
      template <class _policy_list_t> struct reverse_list;
      template <template <class> class ... _policy_ts>
      struct reverse_list<policy_list<_policy_ts...>>{ using type = typename reverse < _policy_ts...>::type; };

      //undup list
      template <class> struct undup_list;
      template <> struct undup_list<policy_list<>>{ using type = policy_list<>; };
      template <template <class> class _head_t, template <class> class ... _tail_t>
      struct undup_list<policy_list<_head_t, _tail_t...>>{
        using unduped_tail = typename undup_list<policy_list<_tail_t...>>::type;
        using unduped_head = typename remove_all_list<_head_t, unduped_tail>::type;
        using type = typename append_list<policy_list<_head_t>, unduped_head>::type;
      };

    }


    template <> struct policy_list<>{
      template<template <class> class ... _append_t>
      using append = policy_list<_append_t...>;

      template <class _append_t>
      using append_list = _append_t;

      template <template <class> class _target_t>
      using exists = std::false_type;

      template <template <class> class _target_t>
      using remove = policy_list<>;

      using reverse = policy_list<>;

      using dedup = policy_list<>;

    };

    template <template <class> class _head_t, template <class> class ... _tail_t>
    struct policy_list<_head_t, _tail_t...> : policy_list<_tail_t...>{

      template<template <class> class ... _append_t>
      using append = policy_list<_head_t, _tail_t..., _append_t...>;

      template <class _append_t>
      using append_list = typename wtf::_::policy_list_impl::append_list<policy_list<_head_t, _tail_t...>, _append_t>::type;

      template <template <class> class _target_t>
      using exists = typename wtf::_::policy_list_impl::exists<_target_t, _head_t, _tail_t...>::type;


      using reverse = typename wtf::_::policy_list_impl::reverse<_head_t, _tail_t...>::type;

      using dedup = typename wtf::_::policy_list_impl::undup_list<policy_list<_head_t, _tail_t...>>::type;
    };


    template <class> struct policy_collector;
    template <> struct policy_collector<policy_list<>>{ using type = policy_list<>; };


    template <template <class> class _head_t, template <class> class ... _tail_t>
    struct policy_collector<policy_list<_head_t, _tail_t...>>{
      using head_reqs = typename policy_traits<_head_t>::requires;
      using tail_list = typename policy_list<_tail_t...>::template append_list<head_reqs>;
      using full_reqs = typename policy_collector<tail_list>::type;
      using type = typename policy_list<_head_t>::template append_list<full_reqs>;
    };

    template <class _impl_t, class _policy_list_t> struct _;
    template <class _impl_t> struct _<_impl_t, policy_list<>> : wtf::window {
      template <typename ... _ParamTs> _(_ParamTs&&...oParam) noexcept : wtf::window(std::forward<_ParamTs>(oParam)...) {}
      void fwd_msg(wtf::window_message& oMsg) { wtf::window::handle_msg(oMsg); }
    };
    template <class _impl_t, template <class> class _head_t, template <class> class ... _tail_t>  
    struct _<_impl_t, policy_list<_head_t, _tail_t...>> : _head_t<_<_impl_t, policy_list<_tail_t...>>> {
      using _super_t = _head_t<_<_impl_t, policy_list<_tail_t...>>>;
      using _next_t = _<_impl_t, policy_list<_tail_t...>>;
      template <typename ... _ParamTs> _(_ParamTs&&...oParam)  : _super_t(std::forward<_ParamTs>(oParam)...) {}
      void fwd_msg(wtf::window_message& oMsg) {
        _super_t::handle_msg(oMsg);
        if (oMsg.bhandled) return;
        _next_t::fwd_msg(oMsg);
      }
    };

    template <class _impl_t, class _policy_list_t> struct construct_hierarchy;
    template <class _impl_t> struct construct_hierarchy<_impl_t, policy_list<>>{ using type = wtf::window; };
    template <class _impl_t, template <class> class _head_t, template <class> class ... _tail_t>
    struct construct_hierarchy<_impl_t, policy_list<_head_t, _tail_t...>>{
      using tail_hierarchy = typename construct_hierarchy<_impl_t, policy_list<_tail_t...>>::type;
      using type = _head_t<tail_hierarchy>;
    };

    template <class _impl_t, template <class> class ... _policy_ts> struct normalized_policies{
      using collected_policies = typename policy_collector<policy_list<_policy_ts...>>::type;
      using reversed_policies = typename collected_policies::reverse;
      using unique_policies = typename reversed_policies::dedup;
      using unique_ordered_policies = typename unique_policies::reverse;
      using type = typename _<_impl_t, unique_ordered_policies>;
    };

  }
}
