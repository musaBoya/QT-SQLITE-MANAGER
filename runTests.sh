
rm -rf test_logs

mkdir test_logs

ninja -v ets_interop_js__generic_as_parameter_ts_to_sts_gtests      > test_logs/out_generic_as_parameter.log 2>&1
ninja -v ets_interop_js__generic_call_params_ts_to_sts_gtests       > test_logs/out_generic_call_params.log 2>&1
ninja -v ets_interop_js__generic_static_sts_to_ts_gtests            > test_logs/out_generic_static.log 2>&1
ninja -v ets_interop_js__getter_arkjs_ets_to_ts_gtests              > test_logs/out_getter_arkjs.log 2>&1
ninja -v ets_interop_js__getter_ts_to_ets_gtests                    > test_logs/out_getter.log 2>&1
ninja -v ets_interop_js__implement_interfaces_ets_to_ts_gtests      > test_logs/out_implement_interfaces.log 2>&1
ninja -v ets_interop_js__import_ets_to_ts_gtests                    > test_logs/out_import.log 2>&1
ninja -v ets_interop_js__indexed_signature_ts_to_sts_gtests         > test_logs/out_indexed_signature.log 2>&1
ninja -v ets_interop_js__indexed_type_ets_to_ts_gtests              > test_logs/out_indexed_type.log 2>&1
ninja -v ets_interop_js__instanceof_ets_to_ts_gtests                > test_logs/out_instanceof.log 2>&1
ninja -v ets_interop_js__interface_ets_to_ts_gtests                 > test_logs/out_interface.log 2>&1
ninja -v ets_interop_js__interfaces_ets_to_ts_gtests                > test_logs/out_interfaces.log 2>&1
ninja -v ets_interop_js__intersection_signature_arg_generic_gtests  > test_logs/out_intersection_signature.log 2>&1



ets_interop_js__generic_as_parameter_sts_to_ts
ets_interop_js__generic_as_parameter_ts_to_sts

ets_interop_js__generic_call_params_sts_to_ts
ets_interop_js__generic_call_params_ts_to_sts

ets_interop_js__generic_static_sts_to_ts
ets_interop_js__static_method_ts_to_sts
ets_interop_js__class_extends_ts_to_sts
ets_interop_js__generic_static_literal_ts_to_sts
ets_interop_js__generic_static_extra_set_ts_to_sts
ets_interop_js__generic_subset_by_ref_static_ts_to_sts
ets_interop_js__generic_subset_by_value_static_ts_to_sts
ets_interop_js__generic_union_static_ts_to_sts
ets_interop_js__generic_user_class_static_ts_to_ets
ets_interop_js__generic_interface_static_ts_to_sts

ets_interop_js__getter_ts_to_ets

ets_interop_js__getter_ets_to_js
ets_interop_js__getter_js_to_ets

ets_interop_js__import_primitive_types_ts_to_sts
ets_interop_js__import_object_ts_to_sts
ets_interop_js__destructured_import_ts_to_sts
ets_interop_js__import_array_ts_to_sts
ets_interop_js__import_function_ts_to_sts
ets_interop_js__import_date_ts_to_sts
ets_interop_js__import_reg_exp_ts_to_sts
ets_interop_js__import_with_alias_ts_to_sts
ets_interop_js__import_default_ts_to_sts
ets_interop_js__import_type_ts_to_sts
ets_interop_js__import_class_ts_to_sts

ets_interop_js_indexed_type

ets_interop_js__instanceof

ets_interop_js__interface_ets_to_ts
ets_interop_js__interface_ts_to_ets