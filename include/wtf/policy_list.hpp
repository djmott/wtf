#pragma once

namespace wtf{

  namespace _{
    namespace policy_list_impl{
      //determine if two policies are identical
      template <template <typename> typename, template <typename> typename> struct is_same_policy : std::false_type{};
      template <template <typename> typename _PolicyT> struct is_same_policy<_PolicyT, _PolicyT> : std::true_type{};


      //append a policy_list to an existing policy_list
      template <typename _AppendList, typename _SourceList> struct append_list;
      template <template <typename> typename ... _AppendListTs, template <typename> typename ... _SourceListTs>
      struct append_list<policy_list<_AppendListTs...>, policy_list<_SourceListTs...>>{
        using type = policy_list<_SourceListTs..., _AppendListTs...>;
      };

      //see if a policy exists in a parameter pack
      template <template <typename> typename _TargetT, template <typename> typename ... _Policies> struct exists;
      template <template <typename> typename _TargetT> struct exists<_TargetT> : std::false_type{};

      template <template <typename> typename _TargetT, template <typename> typename ... _TailT>
      struct exists<_TargetT, _TargetT, _TailT...> : std::true_type{};

      template <template <typename> typename _TargetT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct exists<_TargetT, _HeadT, _TailT...> : exists<_TargetT, _TailT...>{};

      //remove the first occurrence of a policy in a parameter pack
      template <template <typename> typename _TargetT, template <typename> typename ... _Policies> struct remove;
      template <template <typename> typename _TargetT> struct remove<_TargetT>{ using type = policy_list<>; };
      template <template <typename> typename _TargetT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct remove<_TargetT, _HeadT, _TailT...>{
        using removed_tail = typename remove<_TargetT, _TailT...>::type;
        using type = typename append_list<policy_list<_HeadT>, removed_tail>::type;
      };
      template <template <typename> typename _TargetT, template <typename> typename ... _TailT>
      struct remove<_TargetT, _TargetT, _TailT...>{
        using type = policy_list<_TailT...>;
      };

      //remove all occurrences of a policy from a parameter pack
      template <template <typename> typename _TargetT, template <typename> typename ... _Policies> struct remove_all;
      template <template <typename> typename _TargetT> struct remove_all<_TargetT>{ using type = policy_list<>; };



      template <bool, template <typename> typename _TargetT, template <typename> typename ... _Policies> struct remove_all_b;

      template <template <typename> typename _TargetT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct remove_all_b<true, _TargetT, _HeadT, _TailT...>{
        using type = typename remove_all<_TargetT, _TailT...>::type;
      };

      template <template <typename> typename _TargetT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct remove_all_b<false, _TargetT, _HeadT, _TailT...>{
        using tmp_tail = typename remove_all<_TargetT, _TailT...>::type;
        using type = typename append_list<tmp_tail, policy_list<_HeadT>>::type;
      };


      template <template <typename> typename _TargetT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct remove_all<_TargetT, _HeadT, _TailT...> : remove_all_b< is_same_policy<_TargetT, _HeadT>::value, _TargetT, _HeadT, _TailT...>{};


      //remove all from occurrences of a policy from a policy list
      template <template <typename> typename _TargetT, typename _PolicyListT> struct remove_all_list;
      template <template <typename> typename _TargetT, template <typename> typename ... _Policies>
      struct remove_all_list<_TargetT, policy_list<_Policies...>>{ using type = typename remove_all<_TargetT, _Policies...>::type; };


      //reverse order of policy parameter pack
      template <template <typename> typename ... _Policies> struct reverse;
      template <> struct reverse<>{ using type = policy_list<>; };

      template <template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct reverse<_HeadT, _TailT...>{
        using reversed_tail = typename reverse<_TailT...>::type;
        using type = typename append_list<policy_list<_HeadT>, reversed_tail>::type;
      };

      //reverse policy list
      template <typename _PolicyListT> struct reverse_list;
      template <template <typename> typename ... _Policies>
      struct reverse_list<policy_list<_Policies...>>{ using type = typename reverse < _Policies...>::type; };

      //undup list
      template <typename> struct undup_list;
      template <> struct undup_list<policy_list<>>{ using type = policy_list<>; };
      template <template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct undup_list<policy_list<_HeadT, _TailT...>>{
        using unduped_tail = typename undup_list<policy_list<_TailT...>>::type;
        using unduped_head = typename remove_all_list<_HeadT, unduped_tail>::type;
        using type = typename append_list<unduped_head, policy_list<_HeadT>>::type;
      };

      //remove dups
      template <template <typename> typename ... _Policies> struct dedup;
      template <template <typename> typename _HeadT, template <typename> typename ... _TailT>
      struct dedup<_HeadT, _TailT...>{
        using reverse_1 = typename reverse<_HeadT, _TailT...>::type;
        using unduped = typename undup_list<reverse_1>::type;
        using type = typename reverse_list<unduped>::type;
      };


    }
  }


  template <> struct policy_list<>{
    template<template <typename> typename ... _AppendT>
    using append = policy_list<_AppendT...>;

    template <typename _AppendT>
    using append_list = typename _::policy_list_impl::append_list<_AppendT, policy_list<>>::type;

    template <template <typename> typename _TargetT>
    using exists = std::false_type;

    template <template <typename> typename _TargetT>
    using remove = policy_list<>;

    using reverse = policy_list<>;

    using dedup = policy_list<>;

  };

  template <template <typename> typename _HeadT, template <typename> typename ... _TailT>
  struct policy_list<_HeadT, _TailT...> : policy_list<_TailT...>{

    template<template <typename> typename ... _AppendT>
    using append = policy_list<_HeadT, _TailT..., _AppendT...>;

    template <typename _AppendT>
    using append_list = typename _::policy_list_impl::append_list<_AppendT, policy_list<_HeadT, _TailT...>>::type;

    template <template <typename> typename _TargetT>
    using exists = typename _::policy_list_impl::exists<_TargetT, _HeadT, _TailT...>::type;

    template <template <typename> typename _TargetT>
    using remove = typename _::policy_list_impl::remove<_TargetT, _HeadT, _TailT...>::type;

    using reverse = typename _::policy_list_impl::reverse<_HeadT, _TailT...>::type;

    using dedup = typename _::policy_list_impl::dedup<_HeadT, _TailT...>::type;
  };


  template <typename> struct policy_collector;
  template <> struct policy_collector<policy_list<>>{ using type = policy_list<>; };
  template <template <typename> typename _HeadT, template <typename> typename ... _TailT> 
  struct policy_collector<policy_list<_HeadT, _TailT...>>{
    using head_reqs = typename policy_traits<_HeadT>::requires;
    using tail_list = typename policy_list<_TailT...>::template append_list<head_reqs> ;
//    using tail_with_reqs = typename tail_list::template append_list<head_reqs>;
    //using tail_with_reqs = typename head_reqs::template append<_TailT...>;
    using full_reqs = typename policy_collector<tail_list>::type;
    using type = typename policy_list<_HeadT>::template append_list<full_reqs>;
  };


  template <typename _ImplT, typename _PolicyListT> struct construct_hierarchy;
  template <typename _ImplT> struct construct_hierarchy<_ImplT, policy_list<>>{ using type = wtf::window<_ImplT>; };
  template <typename _ImplT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
  struct construct_hierarchy<_ImplT, policy_list<_HeadT, _TailT...>>{
    using tail_hierarchy = typename construct_hierarchy<_ImplT, policy_list<_TailT...>>::type;
    using type = _HeadT<tail_hierarchy>;
  };

  template <typename _ImplT, template <typename> typename ... _Policies> struct normalized_policies{
    using collected_policies = typename policy_collector<policy_list<_Policies...>>::type;
    using reversed_policies = typename collected_policies::reverse;
    using unique_policies = typename reversed_policies::dedup;
    using unique_ordered_policies = typename unique_policies::reverse;
    using type = typename construct_hierarchy<_ImplT, unique_ordered_policies>::type;
  };

}