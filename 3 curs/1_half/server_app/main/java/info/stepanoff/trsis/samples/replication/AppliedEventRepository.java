package info.stepanoff.trsis.samples.replication;

import org.springframework.data.jpa.repository.JpaRepository;

public interface AppliedEventRepository extends JpaRepository<AppliedEvent, String> {
}
