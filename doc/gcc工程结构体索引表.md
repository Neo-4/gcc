## struct
### gcc_targetm_common
    定义：common/conmon-target.h:67
    根据common-target.def文件生成。内容如下：
    struct gcc_targetm_common {
        bool (* handle_option) (struct gcc_options *opts, struct gcc_options *opts_set, const struct cl_decoded_option *decoded, location_t loc);
        void (* option_init_struct) (struct gcc_options *opts);
        const struct default_options * option_optimization_table;
        void (* option_default_params) (void);
        int default_target_flags;
        enum unwind_info_type (* except_unwind_info) (struct gcc_options *opts);
        bool (* supports_split_stack) (bool report, struct gcc_options *opts);
        bool unwind_tables_default;
        bool have_named_sections;
        bool always_strip_dotdot;
    };
#### targetm_common
    声明：common/conmon-target.h:69
    定义：gcc\common\config\i386\i386-common.c:1233
    初始化内容如下：
    {
        ix86_handle_option,
        ix86_option_init_struct,
        ix86_option_optimization_table,
        hook_void_void,
        (TARGET_DEFAULT | TARGET_SUBTARGET_DEFAULT | TARGET_TLS_DIRECT_SEG_REFS_DEFAULT)
        i386_except_unwind_info,
        ix86_supports_split_stack,
        false,
        true,
        false
    }
### obstack
    定义：include/obstack.h:163
#### opts_obstack
    定义：opts-common.c:774
    
    

