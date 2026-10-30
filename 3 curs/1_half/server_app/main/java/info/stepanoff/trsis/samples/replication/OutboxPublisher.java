package info.stepanoff.trsis.samples.replication;

import java.util.concurrent.TimeUnit;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
@ConditionalOnProperty(name = "app.replication.enabled", havingValue = "true")
public class OutboxPublisher {
    private final OutboxRepository outbox;
    private final KafkaTemplate<String, String> kafka;
    @Value("${app.replication.topic}")
    private String topic;

    @Scheduled(fixedDelayString = "${app.replication.publish-delay-ms:250}")
    public void publish() {
        for (OutboxEvent event : outbox.findTop100ByOrderByCreatedAtAscIdAsc()) {
            try {
                kafka.send(topic, event.getPropertyId().toString(), event.getPayload())
                        .get(10, TimeUnit.SECONDS);
                outbox.deleteById(event.getId());
            } catch (Exception exception) {
                if (exception instanceof InterruptedException) Thread.currentThread().interrupt();
                log.warn("Kafka delivery pending for event {}: {}", event.getId(), exception.toString());
                return; // Retain the event for retry; do not overtake it with newer events.
            }
        }
    }
}
