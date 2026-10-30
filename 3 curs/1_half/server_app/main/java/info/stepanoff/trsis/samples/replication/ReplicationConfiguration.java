package info.stepanoff.trsis.samples.replication;

import java.util.UUID;
import org.apache.kafka.clients.admin.NewTopic;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.kafka.listener.DefaultErrorHandler;
import org.springframework.util.backoff.FixedBackOff;
import org.springframework.scheduling.annotation.EnableScheduling;

@Configuration
@EnableScheduling
@ConditionalOnProperty(name = "app.replication.enabled", havingValue = "true")
public class ReplicationConfiguration {
    @Bean
    DefaultErrorHandler replicationErrorHandler() {
        return new DefaultErrorHandler(new FixedBackOff(1000, FixedBackOff.UNLIMITED_ATTEMPTS));
    }

    @Bean
    NewTopic propertyChanges(@Value("${app.replication.topic}") String topic) {
        // One partition gives every replica the same order, including conflicting edits.
        return TopicBuilder.name(topic).partitions(1).replicas(1).build();
    }

    @Bean("replicationGroup")
    String replicationGroup(@Value("${app.instance-id}") String instance) {
        // H2 is empty at every restart: replay the log with a fresh consumer group.
        return "properties-" + instance + "-" + UUID.randomUUID();
    }
}
