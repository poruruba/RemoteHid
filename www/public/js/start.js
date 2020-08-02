'use strict';

//var vConsole = new VConsole();

const remote_url = 'http://【M5StickCのURL】';
const COOKIE_EXPIRE = 365;

var macro_list = [
    {
        title: "デスクトップ切替(←)",
        keys : [
            {
                "type": "code",
                "code": [0x50, 0x00, 0x00, 0x00, 0x00, 0x00],
                "mod": [0, 3]
            },
        ]
    },
    {
        title: "デスクトップ切替(→)",
        keys : [
            {
                "type": "code",
                "code": [0x4f, 0x00, 0x00, 0x00, 0x00, 0x00],
                "mod": [0, 3]
            },
        ]
    },
    {
        title: "Hello World",
        keys : [
            {
                "type": "text",
                "text": "Hello World"
            }
        ]
    }
];

var vue_options = {
    el: "#top",
    data: {
        progress_title: '', // for progress-dialog

        remote_url: remote_url,
        macro_list: macro_list,
        editing_macro_index: -2,
        editing_macro: null,
        newing_macro: null,
        dialog_content: {},
        mod_list: mod_list,
        keyid_list: keyid_list,
    },
    computed: {
    },
    methods: {
        select_macro: function(){
            if( this.editing_macro_index >= 0 ){
                this.editing_macro = JSON.parse(JSON.stringify(this.macro_list[this.editing_macro_index]));
            }else if( this.editing_macro_index == -1 ){
                this.editing_macro = {
                    title: "No title",
                    keys: []
                };
            }else{
                this.editing_macro = null;
            }
        },
        append_key: function(){
            this.dialog_open("#append_select_dialog");
        },
        dialog_custom_close: function(result){
            this.dialog_close("#append_select_dialog");
            if( result != 0 )
                return;
            if( this.dialog_content.type == "text")
                this.editing_macro.keys.push({ type: "text", text: "" });
            else if( this.dialog_content.type == "code")
                this.editing_macro.keys.push({ type: "code", code: [0x00, 0x00, 0x00, 0x00, 0x00, 0x00], mod: [] });
            this.save_cookie();
        },
        delete_key: function(index){
            if( !confirm("本当に削除しますか？") )
                return;

            Vue.delete(this.editing_macro.keys, index);
            this.save_cookie();
        },
        update_key: function(){
            if( this.editing_macro_index >= 0 )
                this.macro_list[this.editing_macro_index] = this.editing_macro;
            else
                this.macro_list.push(this.editing_macro);
            this.editing_macro = null;
            this.editing_macro_index = -2;
            this.save_cookie();
            alert("追加/更新しました。");
        },
        delete_macro: function(index){
            if( !confirm("本当に削除しますか？") )
                return;

            this.editing_macro = null;
            this.editing_macro_index = -2;
            Vue.delete(this.macro_list, index);
            this.save_cookie();
        },
        do_send: function(index){
            var params = {
                keys: []
            };
            var macro = this.macro_list[index];
            for( var i = 0 ; i < macro.keys.length ; i++ ){
                if( macro.keys[i].type == 'code'){
                    var key_code = macro.keys[i];
                    var key = {
                        type: "code",
                        mod: 0,
                    };
                    for( var j = 0 ; j < key_code.mod.length ; j++ )
                        key.mod |= (0x01 << key_code.mod[j]);
                    var code_list = [];
                    for( var j = 0 ; j < key_code.code.length ; j++ ){
                        if( key_code.code[j] != 0x00 )
                            code_list.push(key_code.code[j])
                    }
                    key.code = bufferToHex(code_list);
                    params.keys.push(key);
                }else if( macro.keys[i].type == 'text'){
                    params.keys.push(macro.keys[i]);
                }
            }
            console.log(params);
            do_get(this.remote_url + '/putkey', { params: btoa(JSON.stringify(params)) } );
        },
        save_cookie: function(){
            Cookies.set('macros', JSON.stringify(this.macro_list), { expires: COOKIE_EXPIRE });
        }
    },
    created: function(){
    },
    mounted: function(){
        proc_load();

        var macros = Cookies.get('macros');
        if( macros !== undefined )
            this.macro_list = JSON.parse(macros);
    }
};
vue_add_methods(vue_options, methods_bootstrap);
vue_add_components(vue_options, components_bootstrap);
var vue = new Vue( vue_options );

function do_get(url, qs) {
    var params = new URLSearchParams(qs);
    var url2 = new URL(url);
    url2.search = params;
  
    return fetch(url2.toString(), {
        method: 'GET',
      })
      .then((response) => {
        if (!response.ok)
          throw 'status is not 200';
        return response.json();
      });
}

function bufferToHex(buffer) {
    return Array
        .from (new Uint8Array (buffer))
        .map (b => b.toString (16).padStart (2, "0"))
        .join ("");
}