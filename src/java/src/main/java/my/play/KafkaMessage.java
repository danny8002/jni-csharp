package my.play;

public class KafkaMessage {
    public String key;
    public byte[] value;
    public String topic;
    public int partition;
    public long offset;
    public String timestamp;
}
