
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyTest
{
    public static class UDFs
    {
        public static byte[] Run(KafkaMessage msg)
        {
            Console.WriteLine("CSharp: Value.length=" + msg.Value.Length);
            for (var i= 0;i < msg.Value.Length; ++i)
            {
                sbyte b = (sbyte) msg.Value[i];

                Console.Write((int)(b) + "-");
            }
            Console.WriteLine();
            Console.WriteLine("CSharp: Value=" + Encoding.UTF8.GetString(msg.Value));

            var str = JsonConvert.SerializeObject(msg);

            Console.WriteLine("CSharp: " + str);

            return Encoding.UTF8.GetBytes(str);
        }
    }

    public class KafkaMessage
    {
        public string Key;
        public byte[] Value;
        public string Topic;
        public int Partition;
        public long Offset;
        public string Timestamp;
    }

  
}
