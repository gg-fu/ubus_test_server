#include "ubus_server.h"
#define DBG_UBUS_SERVER 2
static int id;
static struct blob_buf b;
static struct ubus_context *ctx;
enum {
        name,
        age,
        address,
        __EXCUE_MAX
};

static const struct blobmsg_policy get_address_policy[__EXCUE_MAX] = {
  [name] = { .name = "name", .type = BLOBMSG_TYPE_STRING},
  [age] = { .name = "age", .type = BLOBMSG_TYPE_INT8},
  [address] = { .name = "address", .type = BLOBMSG_TYPE_STRING},
};

static int get_address(struct ubus_context *uctx, struct ubus_object *obj,
                      struct ubus_request_data *req, const char *method,
                      struct blob_attr *msg)
{

        struct blob_attr *tb[__EXCUE_MAX];
        char *name_;
        int age_;
        char *address_;
        void *dtable;

        blobmsg_parse(get_address_policy, __EXCUE_MAX, tb, blob_data(msg), blob_len(msg));

        if (!tb[name] || !tb[age] || !tb[address])
                return UBUS_STATUS_INVALID_ARGUMENT;

        name_ = blobmsg_get_string(tb[name]);
        address_ = blobmsg_get_string(tb[address]);
        age_ = blobmsg_get_u8(tb[age]);
        printf("name:%s,address:%s,age:%d\n",name_,address_,age_);
}

static const struct ubus_method address_methods[] = {
        UBUS_METHOD("name", get_address, get_address_policy),
};

static struct ubus_object_type address_object_type =
        UBUS_OBJECT_TYPE("server", address_methods);

static struct ubus_object address_object = {
        .name = "server",
        .type = &address_object_type,
        .methods = address_methods,
        .n_methods = ARRAY_SIZE(address_methods),
        //.subscribe_cb = client_subscribe_cb,
};
static void report_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
	printf("report_cb\n");
}

int Send_user_message()
{
        blobmsg_buf_init(&b);
        blobmsg_add_string(&b, "name","Bruvin");
        blobmsg_add_u8(&b, "age",24);
        blobmsg_add_string(&b,"address","guangdong shenzhen");

        //ubus_invoke(ctx, id, "report", b.head, report_cb, 0, 500);
	
	ubus_notify(ctx,  &address_object, "server", b.head, -1);
}

void *ubus_server_main(void *arg)
{
        int ret;
	char *ubus_socket = (char *)arg;
	for(;;)
	{
       		uloop_init();
        	ctx = ubus_connect(ubus_socket);
        	if (!ctx) {
			printf("ubus_connect failed\n");
               	 	return NULL;
        	}

        	ubus_add_uloop(ctx);

        	ret = ubus_add_object(ctx, &address_object);
        	if (ret)
			printf("ubus_add_object failed\n");
        	uloop_run();
	}

}

void Bru_create_pthread(void *(*func)(void *),void *arg)
{
        pthread_t thread;
        pthread_attr_t attr;
        int ret;
        pthread_attr_init(&attr);

        ret = pthread_create(&thread,&attr,func,arg);
        if (ret!=0)
        {
		printf("pthread_create failed\n");
                exit(1);
        }
}


void ubus_thread(char *arg)
{
	Bru_create_pthread(ubus_server_main,(void *)arg);	
}
