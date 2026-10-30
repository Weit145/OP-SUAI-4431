package info.stepanoff.trsis.samples.replication;

import java.util.List;
import org.springframework.data.jpa.repository.JpaRepository;

public interface OutboxRepository extends JpaRepository<OutboxEvent, String> {
    List<OutboxEvent> findTop100ByOrderByCreatedAtAscIdAsc();
}
