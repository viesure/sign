package io.viesure.iot.sign.services;

import com.amazonaws.services.iot.client.AWSIotException;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.amazonaws.services.iot.client.AWSIotQos;
import com.amazonaws.services.iot.client.sample.sampleUtil.SampleUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;

@Service
public class SignService {

    private final Logger LOGGER = LoggerFactory.getLogger(SignService.class);

    @Value("${clientEndpoint}")
    private String clientEndpoint;

    @Value("${clientId}")
    private String clientId;

    @Value("${certificateFile}")
    private String certificateFile;

    @Value("${privateKeyFile}")
    private String privateKeyFile;

    @Value("${topic}")
    private String topic;

    private AWSIotMqttClient client;

    @PostConstruct
    public void init() throws AWSIotException {

        SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(certificateFile, privateKeyFile);

        client = new AWSIotMqttClient(clientEndpoint, clientId, pair.keyStore, pair.keyPassword);
        client.connect();
    }

    public void changeRGB(int red, int green, int blue) {

        try {
            String payload = normalize(red) + "," + normalize(green) + "," + normalize(blue);

            LOGGER.info(payload);
            client.publish(topic, AWSIotQos.QOS1, payload);
        } catch (AWSIotException e) {
            LOGGER.error("Error", e);
        }

    }

    /**
     * Our micro controller accepts 8 bit values from 0 to 255 and we use a amplifier with a common anode so we have to invert the values
     *
     * @param value
     * @return
     */
    private int normalize(int value) {
        return 255 - (value - 1);
    }
}
