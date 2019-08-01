package my.play;


import com.google.gson.*;

import java.lang.reflect.Type;
import java.util.Base64;
import java.nio.charset.StandardCharsets;

public class Main {
    public static void main(String[] args) throws Exception {

        String value = "我是石头abc啊";

        KafkaMessage msg = new KafkaMessage();
        msg.key ="my_key";
        msg.value = value.getBytes(StandardCharsets.UTF_8);
        msg.topic = "my_topic";
        msg.partition = 2;
        msg.offset = 1234567898765L;
        msg.timestamp="abdFDS";

        byte[] bytes = UDFs.run(msg);

        Gson gson = new GsonBuilder()
                .registerTypeAdapter(byte[].class, new ByteArrayBase64Adapter())
                .setFieldNamingPolicy(FieldNamingPolicy.UPPER_CAMEL_CASE)
                .create();

        System.out.println("In java: " + gson.toJson(msg));
        for(int i=0;i<msg.value.length;++i){
            byte b = msg.value[i];
            System.out.print((int)b +"-");
        }

        System.out.println();

        String str = new String(bytes, StandardCharsets.UTF_8);
        KafkaMessage ret = gson.fromJson(str, KafkaMessage.class);

        System.out.println(gson.toJson(ret));
    }


    public static class ByteArrayBase64Adapter implements JsonSerializer<byte[]>, JsonDeserializer<byte[]> {

        @Override
        public JsonElement serialize(byte[] src, Type typeOfSrc,
                                     JsonSerializationContext context) {

            return new JsonPrimitive(Base64.getEncoder().encodeToString(src));

        }

        @Override
        public byte[] deserialize(JsonElement jsonElement, Type type, JsonDeserializationContext jsonDeserializationContext) throws JsonParseException {

            JsonPrimitive v = (JsonPrimitive)jsonElement;

            return Base64.getDecoder().decode(v.getAsString());

        }
    }
}
